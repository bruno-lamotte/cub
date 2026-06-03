#!/usr/bin/env python3
"""
Cub3D Asset Converter & Optimizer (v2.1)
----------------------------------------
Converts source PNGs from 'ignore/assets/' into optimized game-ready assets in 'assets/'.

Features:
- Sequence Detection: Automatically detects animations based on numeric suffixes (e.g., walk01, walk02).
- High-Quality Quantization: Uses ffmpeg's palettegen/paletteuse for pro-grade 8-bit style.
- DXPM Compression: Custom "True Delta" format for animations.
- Auto-Metadata: Generates/updates .anim files for detected sequences.
"""
import os, subprocess, sys, tempfile, shutil, re

def run_cmd(cmd):
    p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = p.communicate()
    if p.returncode != 0: print(f"CMD ERR: {cmd}\n{stderr.decode()}")
    return p.returncode, stdout, stderr

def get_pam_data(path):
    cmd = f"ffmpeg -y -i {path} -f image2pipe -vcodec pam -"
    rc, stdout, stderr = run_cmd(cmd)
    if rc != 0: return None, None, None
    lines = stdout.split(b"\n")
    w, h, d, header_end = 0, 0, 4, 0
    for line in lines:
        if line.startswith(b"WIDTH"): w = int(line.split()[1])
        elif line.startswith(b"HEIGHT"): h = int(line.split()[1])
        elif line.startswith(b"DEPTH"): d = int(line.split()[1])
        elif line.startswith(b"ENDHDR"):
            header_end = stdout.find(b"ENDHDR") + 7
            break
    if not w or not h: return None, None, None
    raw = list(stdout[header_end:])
    rgba = []
    for i in range(0, len(raw), d):
        chunk = raw[i:i+d]
        if len(chunk) < d: break
        r_val, g_val, b_val = chunk[0], chunk[1], chunk[2]
        if g_val > 80 and g_val > r_val + 30 and g_val > b_val + 30:
            rgba.append((0, 0, 0, 0))
        elif d == 3: rgba.append((r_val, g_val, b_val, 255))
        else: rgba.append((r_val, g_val, b_val, chunk[3]))
    if len(rgba) > w * h: rgba = rgba[:w*h]
    while len(rgba) < w * h: rgba.append((0,0,0,0))
    return w, h, rgba

def write_xpm_header(f, w, h, p_map, cpp):
    f.write(f"\"{w} {h} {len(p_map)} {cpp}\",\n")
    for col, tok in p_map.items():
        if col[3] == 0: f.write(f"\"{tok} c None\",\n")
        else: f.write(f"\"{tok} c #{col[0]:02x}{col[1]:02x}{col[2]:02x}\",\n")

def convert_to_xpm(path, w, h, pix, p_map, cpp):
    with open(path, "w") as f:
        f.write("/* XPM */\nstatic char *xpm[] = {\n")
        write_xpm_header(f, w, h, p_map, cpp)
        for y in range(h):
            row = "".join(p_map.get(pix[y*w+x], p_map[(0,0,0,0)]) for x in range(w))
            f.write(f"\"{row}\",\n")
        f.write("};\n")

def convert_to_dxpm(path, w, h, pix, p_map, cpp, p_pix, p_name):
    with open(path, "w") as f:
        f.write("/* DXPM */\nPREV {p_name}\n".format(p_name=p_name))
        write_xpm_header(f, w, h, p_map, cpp)
        for y in range(h):
            pr, cr = p_pix[y*w:(y+1)*w], pix[y*w:(y+1)*w]
            if pr == cr: continue
            f.write(f"R{y} ")
            x = 0
            while x < w:
                if pr[x] != cr[x]:
                    s = x
                    while x < w and pr[x] != cr[x]: x += 1
                    toks = "".join(p_map.get(cr[i], p_map[(0,0,0,0)]) for i in range(s, x))
                    f.write(f"{s},{x-s} {toks} ")
                else: x += 1
            f.write("\n")

def update_anim_file(dir_p, ox, oy):
    for anim in [f for f in os.listdir(dir_p) if f.endswith(".anim")]:
        path = os.path.join(dir_p, anim)
        with open(path, "r") as f: lines = f.readlines()
        nl = [f"offset_x {ox}\n", f"offset_y {oy}\n"]
        for l in lines:
            if "offset_" in l: continue
            pts = l.split()
            if len(pts) == 2 and (pts[0].endswith(".xpm") or pts[0].endswith(".dxpm")):
                base = pts[0].split(".")[0]
                if os.path.exists(os.path.join(dir_p, base + ".dxpm")): l = f"{base}.dxpm {pts[1]}\n"
                elif os.path.exists(os.path.join(dir_p, base + ".xpm")): l = f"{base}.xpm {pts[1]}\n"
            nl.append(l)
        with open(path, "w") as f: f.writelines(nl)

def process_animation(t_dir, pngs, sc):
    d_dir = t_dir.replace("ignore/", "")
    os.makedirs(d_dir, exist_ok=True)
    if not pngs: return
    with tempfile.TemporaryDirectory() as tmp:
        pal_p, sf = os.path.join(tmp, "p.png"), (f"scale={sc}" if sc else "")
        run_cmd(f"ffmpeg -y -i {os.path.join(t_dir, pngs[0])} -vf '{sf}{',' if sc else ''}palettegen=max_colors=255' -frames:v 1 {pal_p}")
        q_frames, valid = [], []
        for p in pngs:
            dst = os.path.join(tmp, p)
            lavfi = f"scale={sc}[x];[x][1:v]paletteuse" if sc else "paletteuse"
            run_cmd(f"ffmpeg -y -i {os.path.join(t_dir, p)} -i {pal_p} -lavfi '{lavfi}' -frames:v 1 {dst}")
            w, h, pix = get_pam_data(dst)
            if pix: q_frames.append(pix); valid.append(p); width, height = w, h
        if not q_frames: return
        min_x, min_y, max_x, max_y = width, height, 0, 0
        for f_pix in q_frames:
            for i, (r,g,b,a) in enumerate(f_pix):
                if a > 0:
                    x, y = i % width, i // width
                    min_x, min_y, max_x, max_y = min(min_x, x), min(min_y, y), max(max_x, x), max(max_y, y)
        bx1, by1, bx2, by2 = (0,0,1,1) if max_x < min_x else (min_x, min_y, max_x+1, max_y+1)
        cw, ch = bx2-bx1, by2-by1
        _, _, pal_pix = get_pam_data(pal_p)
        colors = []
        for p in pal_pix:
            if p not in colors: colors.append(p)
        t_str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&()*+,-./:;<=>?[\\]^_`{|}~"
        cpp = 1 if len(colors)+1 <= len(t_str) else 2
        def get_tok(idx): return t_str[idx] if cpp == 1 else t_str[idx//len(t_str)]+t_str[idx%len(t_str)]
        p_map = {(0,0,0,0): "."*cpp}
        for i, c in enumerate(colors): p_map[c] = get_tok(i+1)
        prev_pix, prev_name = None, None
        for i, p in enumerate(valid):
            raw = q_frames[i]
            cr = [raw[y*width+x] for y in range(by1, by2) for x in range(bx1, bx2)]
            out_p = os.path.join(d_dir, p.replace(".png", ".xpm" if i == 0 else ".dxpm"))
            if i == 0: convert_to_xpm(out_p, cw, ch, cr, p_map, cpp)
            else: convert_to_dxpm(out_p, cw, ch, cr, p_map, cpp, prev_pix, prev_name)
            prev_pix, prev_name = cr, os.path.basename(out_p)
        anim_found = False
        for f in os.listdir(t_dir):
            if f.endswith(".anim"): 
                shutil.copy(os.path.join(t_dir, f), os.path.join(d_dir, f))
                anim_found = True
        if not anim_found:
            with open(os.path.join(d_dir, os.path.basename(t_dir)+".anim"), "w") as f:
                f.write("original_fps 60\n")
                for i, p in enumerate(valid): f.write(f"{p.replace('.png', '.xpm' if i==0 else '.dxpm')} 1\n")
        update_anim_file(d_dir, int((bx1+bx2)/2 - width/2), by1)

def process_static(t_p, sc):
    d_p = t_p.replace("ignore/", "").replace(".png", ".xpm")
    os.makedirs(os.path.dirname(d_p), exist_ok=True)
    with tempfile.TemporaryDirectory() as tmp:
        pal_p, q_p = os.path.join(tmp, "p.png"), os.path.join(tmp, "q.png")
        sf = (f"scale={sc}" if sc else "")
        run_cmd(f"ffmpeg -y -i {t_p} -vf '{sf}{',' if sc else ''}palettegen=max_colors=255' -frames:v 1 {pal_p}")
        lavfi = f"scale={sc}[x];[x][1:v]paletteuse" if sc else "paletteuse"
        run_cmd(f"ffmpeg -y -i {t_p} -i {pal_p} -lavfi '{lavfi}' -frames:v 1 {q_p}")
        w, h, pix = get_pam_data(q_p)
        if not pix: return
        _, _, pal_pix = get_pam_data(pal_p)
        colors = []
        for p in pal_pix:
            if p not in colors: colors.append(p)
        t_str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&()*+,-./:;<=>?[\\]^_`{|}~"
        cpp = 1 if len(colors)+1 <= len(t_str) else 2
        def get_tok(idx): return t_str[idx] if cpp == 1 else t_str[idx//len(t_str)]+t_str[idx%len(t_str)]
        p_map = {(0,0,0,0): "."*cpp}
        for i, c in enumerate(colors): p_map[c] = get_tok(i+1)
        convert_to_xpm(d_p, w, h, pix, p_map, cpp)

def main():
    if len(sys.argv) < 2: return
    rp, sc = sys.argv[1], (sys.argv[2] if len(sys.argv) > 2 else None)
    if rp.endswith((".mp4", ".mov", ".avi", ".mkv")):
        print(f">>> Video file detected: {rp}")
        base = os.path.basename(rp).split(".")[0]
        t_dir = f"ignore/assets/{base}"
        os.makedirs(t_dir, exist_ok=True)
        for f in os.listdir(t_dir):
            if f.endswith(".png"):
                os.remove(os.path.join(t_dir, f))
        run_cmd(f"ffmpeg -y -i {rp} -vf 'fps=1.5' {t_dir}/{base}%02d.png")
        rp = t_dir
    if os.path.isfile(rp): process_static(rp, sc)
    else:
        for r, d, f in os.walk(rp):
            pngs = sorted([f_ for f_ in f if f_.endswith(".png") and "sheet" not in f_])
            if not pngs: continue
            # Group by prefix to detect sequences
            sequences = {}
            for p in pngs:
                m = re.match(r"(.*?)(\d+)\.png", p)
                if m:
                    prefix = m.group(1)
                    if prefix not in sequences: sequences[prefix] = []
                    sequences[prefix].append(p)
                else:
                    process_static(os.path.join(r, p), sc)
                    # Clean up any stale animation data
                    d_p = os.path.join(r.replace("ignore/", ""), p.replace(".png", ".dxpm"))
                    if os.path.exists(d_p): os.remove(d_p)
            
            # Clean up stale .anim files in non-sequence dirs
            if not sequences:
                anim_p = os.path.join(r.replace("ignore/", ""), os.path.basename(r) + ".anim")
                if os.path.exists(anim_p): os.remove(anim_p)

            for prefix, seq in sequences.items():
                if len(seq) > 1:
                    print(f">>> Animation sequence detected: {prefix} ({len(seq)} frames) in {r}")
                    process_animation(r, seq, sc)
                else:
                    for p in seq: process_static(os.path.join(r, p), sc)

if __name__ == "__main__": main()

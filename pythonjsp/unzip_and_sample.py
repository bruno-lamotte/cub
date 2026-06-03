import os, zipfile, shutil

def main():
    t_dir = "ignore/assets/monstre"
    if os.path.exists(t_dir):
        shutil.rmtree(t_dir)
    os.makedirs(t_dir, exist_ok=True)

    zip_path = "pythonjsp/chroma-keyed-video.zip"
    print(">>> Unzipping chroma-keyed-video.zip...")
    with zipfile.ZipFile(zip_path, 'r') as zip_ref:
        zip_ref.extractall(t_dir)

    pngs = sorted([f for f in os.listdir(t_dir) if f.endswith(".png")])
    print(f">>> Total extracted frames: {len(pngs)}")

    # Keep every 4th frame to fit the 128-frame buffer limit
    keep_indices = list(range(0, len(pngs), 4))
    keep_names = set(pngs[i] for i in keep_indices)

    deleted = 0
    for f in pngs:
        if f not in keep_names:
            os.remove(os.path.join(t_dir, f))
            deleted += 1

    print(f">>> Kept {len(keep_names)} frames, deleted {deleted} frames.")

if __name__ == "__main__":
    main()

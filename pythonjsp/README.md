# Cub3D Tooling & Asset Pipeline

This directory contains the utilities used to manage, optimize, and convert game assets for the Cub3D engine.

## `convert.py`

The primary asset pipeline tool. It bridges the gap between high-resolution source art (in `ignore/assets/`) and the performance-optimized formats required by the engine.

### Features

- **8-Bit Aesthetic**: Uses high-quality ffmpeg quantization to give the game a consistent, professional retro look.
- **Global Bounding Box**: For animations, the tool calculates the tightest box that contains *all* frames. This ensures that the gun or sprite stays perfectly centered without manually calculating offsets.
- **DXPM (Delta XPM)**: A custom sparse format that stores only changed pixel spans between frames. This reduces animation disk footprint by ~90% and accelerates loading.
- **Resolution Scaling**: Support for downscaling assets (e.g., `320:180`) while maintaining pixel-perfect transparency.

### Usage

```bash
python3 tools/convert.py <input_path> [WxH]
```

- **Input Path**: Can be a single PNG or a directory (recursive).
- **WxH**: Optional. Rescales images to this resolution using nearest-neighbor interpolation.

---

## Technical Spec: DXPM Format

The `.dxpm` format is a text-based sparse diff format designed for Cub3D.

### Structure
```c
/* DXPM */
PREV frame00.xpm    // Path to the base/previous frame
"W H COLORS CPP"     // Standard XPM header (redefined for robustness)
"a c #RRGGBB"        // Palette (Shared with base frame)
R<row> <col>,<len> <tokens> // Sparse row data
```

### Parsing Logic
1.  Load the `PREV` frame pixels into a buffer.
2.  Read the `R` lines.
3.  For each line, jump to `row * Width + col`.
4.  Write `len` pixels using the following `tokens`.
5.  All unchanged pixels in the buffer remain as they were in the previous frame.

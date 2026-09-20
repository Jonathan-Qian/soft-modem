import os
import sys

# 1. Clear GPU paths and bind native system LLVM handles
os.environ["CUDA_VISIBLE_DEVICES"] = ""
os.environ["KMP_DUPLICATE_LIB_OK"] = "TRUE"
os.environ["DRJIT_LIBLLVM_PATH"] = r"C:\Program Files\LLVM\bin\LLVM-C.dll"

# 2. Add LLVM directory path hooks
llvm_path = r"C:\Program Files\LLVM\bin"
if llvm_path not in os.environ["PATH"]:
    os.environ["PATH"] = llvm_path + os.pathsep + os.environ["PATH"]

# 3. Restrict Mitsuba rendering logic to the Mono Polarized CPU framework
try:
    import drjit as dr
    import mitsuba as mi
    if hasattr(dr, 'set_flag'):
        dr.set_flag(dr.JitFlag.ForceOptiX, False)
    mi.set_variant('llvm_ad_mono_polarized')
    print("Mitsuba variant successfully restricted to: LLVM Polarized (CPU)")
except Exception as e:
    print(f"Dr.Jit/Mitsuba configuration notice: {e}")

# 4. Set up UI wrapper and framework interfaces
try:
    import polyscope as ps
    from sionna_rt_gui import AppHolder, DEFAULT_CONFIG_PATH
    from sionna_rt_gui.config import load_config
except ImportError as e:
    print(f"Import failed: {e}")
    sys.exit(1)

if __name__ == "__main__":
    print("Environment locked and balanced. Configuring CPU flags...")
    cfg = load_config(DEFAULT_CONFIG_PATH)
    
    # =====================================================================
    # THE FIX: Force turn off the GPU denoiser to prevent app scaling crashes
    # =====================================================================
    cfg.rendering.use_denoiser = False
    # =====================================================================
    
    # 5. Initialize layout frame space with empty scene string
    app = AppHolder(cfg, scene_filename="")
    
    # 6. Fire up the interactive viewport loop
    ps.show()

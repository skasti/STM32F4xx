Import("env")

def after_build(source, target, env): 
    print("[From Script] Finished building!!")
    env.Execute("python3 uf2conv.py -c -b 0x08010000 -f 0x57755a57 C:/Repo/GRBLHAL_Expatria/STM32F4xx_release/.pio/build/f446re_flexi_cnc/firmware.hex")

env.AddPostAction("buildprog", after_build)


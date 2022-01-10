Import("env", "projenv")
from pathlib import Path

print("===============================================================================================================")
print()
print("===============================================================================================================")
env["PROJECT_SRC_DIR"] = Path(env["PROJECT_DIR"]) / "src" / env["PIOENV"]

if env["PIOENV"] == "onboard_device":
    env.Append(CPPFLAGS=["-rtti"])
elif env["PIOENV"] == "remote_device":
    env.Append(CPPFLAGS=["-rtti"])
elif env["PIOENV"] == "native":
    env.Append(CPPFLAGS=["-std=gnu17"])
else:
    print("Unsupported Pio env!!")

print("===============================================================================================================")
print(env.Dump())
print("===============================================================================================================")

Import("env", "projenv")

env.Append(CXXFLAGS=["-frtti"])
projenv.Append(CXXFLAGS=["-frtti"])

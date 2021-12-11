import targets
import testlib

# Based on spike32.py

class hazard3_hart(targets.Hart):
    xlen = 32
    ram = 0x00000000
    ram_size = 128 * 1024 # amount present on iCEBreaker -- can be more on CXXRTL sim.
    bad_address = 0xf00ba700
    instruction_hardware_breakpoint_count = 0
    reset_vectors = [0xc0]
    link_script_path = "hazard3.lds"
    misa = 0x40801105 # 32IMACX (X because of nonstandard debug CSRs)


class hazard3(targets.Target):
    harts = [hazard3_hart()]
    openocd_config_path = "hazard3-remote-bitbang.cfg"
    timeout_sec = 300
    support_memory_sampling = False # Needs SBA

    def create(self):
        return testlib.GenericSimCmd(sim_cmd=self.sim_cmd)

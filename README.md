# zcu104_ubuntu
## A project to demonstrate Xilinx MPSOC running Ubuntu

The Xilinx Multi-Processor System On Chip (MPSOC) devices provide agile solutions to a large class of problems. One application that is rarely discussed is their use as design verification engines. Many custom logic blocks contain so much complexity that it is impossible to adequately verify them using traditional simulation techniques.  A fast microprocessor tightly coupled to a high performance FPGA fabric is ideal for running logic through vast test sets. Once running, hardware verification can have a six order of magnitude speed increase compared to simulation.

When testing logic it is ideal to have a complete desktop Linux distribution running on the control computer. Rich development and analysis tools become available such as subversion, git, make, Octave, python, gcc, vim, ssh, Apache, bash, ....  

The goal of this project is to demonstrate Ubuntu running on MPSOC for the purpose of logic verification.

### Tasks:
- **Create a simple FPGA Design** - See the readme under fpga for instructions.  There is a block ram and a register file that can be access from a program running under Ubuntu Linux.
- **Boot Ubuntu Linux on the ZynqMP 64 bit Arm processor** - See software/petalinux/readme.txt for instructions.
- **Demonstrate basic access to FPGA features from Linux** - Look in software/app/bram_test.
- **Demonstrate large-scale hardware verification** - We need suggestions on this.  I am thinking about doing some linear algebra, complex matrix inversion stuff in HLS.

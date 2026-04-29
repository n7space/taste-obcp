# taste-obcp
TASTE OBCP component based on Micropython embed port, developed as a part of "Model-Based Execution Platform for Space Applications" project (contract 4000146882/24/NL/KK) financed by the European Space Agency.

The On-Board Control Procedure Engine is based on Micropython (https://micropython.org/), and derived from the "embed" port to maximize compatibility with target platforms, such as x86-64 (for testing/demonstration) and ARM Cortex-M7 (e.g., Microchip SAMV71/SAMRH71). It is intended to be integrated within larger Application Software. Interface requirements, for emitting events, as well as interacting with DataPool, Requests and Reports are driven by MBEP-N7S-EP-SRS document, while its high-level design is described in MBEP-N7S-EP-SDD.

This repository contains a TASTE component that wraps the engine (located in a separate [repository](https://github.com/n7space/n7s-obcp), which is included here as a submodule). The component is ready for (re)use within ESA's [TASTE MBSE Toolchain](https://taste.tools/).

.PHONY: all flat mpy-cross update-test-project

N7OBCP_DIR=n7s-obcp
FLAT_DIR=$(N7OBCP_DIR)/src/flat
TEST_PROJECT_OBCP_DIR=test-project/ObcpTest/work/obcp_engine/C/src

all: flat mpy-cross

flat:
	make -C $(N7OBCP_DIR) flat

mpy-cross:
	make -C $(N7OBCP_DIR) mpy-cross

update-test-project: flat
	cp -v $(FLAT_DIR)/*.h  $(TEST_PROJECT_OBCP_DIR)
	cp -v $(FLAT_DIR)/*.c  $(TEST_PROJECT_OBCP_DIR)
TARGET = RayOS.hex 
PROJECT_ROOT = .

CC = sdcc 
LD = sdcc 

MAKEHEX = packihx
MAKEBIN = makebin -Z

INC_DIR = $(PROJECT_ROOT)/../../../include $(PROJECT_ROOT)/../../../bsp/stc15/libraries $(PROJECT_ROOT)/../../../bsp/stc15/iap15w4k61s4/example $(PROJECT_ROOT)/../../../bsp/stc15/iap15w4k61s4/board $(PROJECT_ROOT)/../../../bsp/stc15/iap15w4k61s4/application
OBJS_DIR = $(PROJECT_ROOT)/../../../bsp/stc15/iap15w4k61s4

CFLAGS = -mmcs51 --model-large
CFLAGS += $(addprefix -I ,$(INC_DIR))

LDFLAGS =

THREAD_SRC = $(PROJECT_ROOT)/../../../src/thread.c
CONTEXT_SRC = $(PROJECT_ROOT)/../../../cpu/8051/context.c
SCHEDULER_SRC = $(PROJECT_ROOT)/../../../src/scheduler.c
SEMAPHORE_SRC = $(PROJECT_ROOT)/../../../src/semaphore.c
MAILBOX_SRC = $(PROJECT_ROOT)/../../../src/mailbox.c
GPIO_SRC = $(PROJECT_ROOT)/../libraries/GPIO.c
TIMER_SRC = $(PROJECT_ROOT)/../libraries/timer.c
USART_SRC = $(PROJECT_ROOT)/../libraries/USART.c
BOARD_SRC = $(PROJECT_ROOT)/board/board.c
MAIN_SRC = $(PROJECT_ROOT)/application/main.c
DEBUG_SRC = $(PROJECT_ROOT)/application/debug.c
EXAMPLE_SRC = $(PROJECT_ROOT)/example/example.c

THREAD_OBJ = $(addprefix $(OBJS_DIR)/, $(patsubst %.c,%.rel,$(notdir $(THREAD_SRC))))
CONTEXT_OBJ = $(addprefix $(OBJS_DIR)/, $(patsubst %.c,%.rel,$(notdir $(CONTEXT_SRC))))
SCHEDULER_OBJ = $(addprefix $(OBJS_DIR)/, $(patsubst %.c,%.rel,$(notdir $(SCHEDULER_SRC))))
SEMAPHORE_OBJ = $(addprefix $(OBJS_DIR)/, $(patsubst %.c,%.rel,$(notdir $(SEMAPHORE_SRC))))
MAILBOX_OBJ = $(addprefix $(OBJS_DIR)/, $(patsubst %.c,%.rel,$(notdir $(MAILBOX_SRC))))
GPIO_OBJ = $(addprefix $(OBJS_DIR)/, $(patsubst %.c,%.rel,$(notdir $(GPIO_SRC))))
TIMER_OBJ = $(addprefix $(OBJS_DIR)/, $(patsubst %.c,%.rel,$(notdir $(TIMER_SRC))))
USART_OBJ = $(addprefix $(OBJS_DIR)/, $(patsubst %.c,%.rel,$(notdir $(USART_SRC))))
BOARD_OBJ = $(addprefix $(OBJS_DIR)/, $(patsubst %.c,%.rel,$(notdir $(BOARD_SRC))))
MAIN_OBJ = $(addprefix $(OBJS_DIR)/, $(patsubst %.c,%.rel,$(notdir $(MAIN_SRC))))
DEBUG_OBJ = $(addprefix $(OBJS_DIR)/, $(patsubst %.c,%.rel,$(notdir $(DEBUG_SRC))))
EXAMPLE_OBJ = $(addprefix $(OBJS_DIR)/, $(patsubst %.c,%.rel,$(notdir $(EXAMPLE_SRC))))

all : hex

$(THREAD_OBJ):
	$(CC) -c $(CFLAGS) $(THREAD_SRC) -o$(OBJS_DIR)/

$(CONTEXT_OBJ):
	$(CC) -c $(CFLAGS) $(CONTEXT_SRC) -o$(OBJS_DIR)/

$(SCHEDULER_OBJ):
	$(CC) -c $(CFLAGS) $(SCHEDULER_SRC) -o$(OBJS_DIR)/

$(SEMAPHORE_OBJ):
	$(CC) -c $(CFLAGS) $(SEMAPHORE_SRC) -o$(OBJS_DIR)/

$(MAILBOX_OBJ):
	$(CC) -c $(CFLAGS) $(MAILBOX_SRC) -o$(OBJS_DIR)/

$(GPIO_OBJ):
	$(CC) -c $(CFLAGS) $(GPIO_SRC) -o$(OBJS_DIR)/

$(TIMER_OBJ):
	$(CC) -c $(CFLAGS) $(TIMER_SRC) -o$(OBJS_DIR)/

$(USART_OBJ):
	$(CC) -c $(CFLAGS) $(USART_SRC) -o$(OBJS_DIR)/

$(BOARD_OBJ):
	$(CC) -c $(CFLAGS) $(BOARD_SRC) -o$(OBJS_DIR)/

$(MAIN_OBJ):
	$(CC) -c $(CFLAGS) $(MAIN_SRC) -o$(OBJS_DIR)/

$(DEBUG_OBJ):
	$(CC) -c $(CFLAGS) $(DEBUG_SRC) -o$(OBJS_DIR)/
	
$(EXAMPLE_OBJ):
	$(CC) -c $(CFLAGS) $(EXAMPLE_SRC) -o$(OBJS_DIR)/

hex:RayOS.ihx
	$(MAKEHEX) RayOS.ihx > $(TARGET)

RayOS.ihx:thread.ihx
	cp thread.ihx RayOS.ihx

thread.ihx:$(THREAD_OBJ) $(CONTEXT_OBJ) $(SCHEDULER_OBJ) $(SEMAPHORE_OBJ) $(MAILBOX_OBJ) $(GPIO_OBJ) $(TIMER_OBJ) $(USART_OBJ) $(BOARD_OBJ) $(MAIN_OBJ) $(DEBUG_OBJ) $(EXAMPLE_OBJ)
	$(LD) $(LDFLAGS) $(THREAD_OBJ) $(CONTEXT_OBJ) $(SCHEDULER_OBJ) $(SEMAPHORE_OBJ) $(MAILBOX_OBJ) $(GPIO_OBJ) $(TIMER_OBJ) $(USART_OBJ) $(BOARD_OBJ) $(MAIN_OBJ) $(DEBUG_OBJ) $(EXAMPLE_OBJ)

bin:RayOS.ihx
	$(MAKEBIN) RayOS.ihx $(patsubst %.hex,%.bin,$(TARGET))

clean:
	rm -rf $(OBJS_DIR)/*.asm $(OBJS_DIR)/*.lst $(OBJS_DIR)/*.rel $(OBJS_DIR)/*.rst $(OBJS_DIR)/*.sym
	rm -f *.bin
	rm -f *.hex
	rm -f *.ihx
	rm -f *.lk
	rm -f *.map
	rm -f *.mem

version:
	$(CC) --version

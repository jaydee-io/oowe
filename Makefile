CXX_FLAGS= -std=c++11 -Wall
CXX=clang++
LD=clang++

##########################################
# OOWe Library
##########################################
OBJS += src/String.o
OBJS += src/Session.o
OBJS += src/HttpSession.o

INCS += -I include
INCS += -I $(CURL_DIR)/include

LIBS += -L $(CURL_DIR)/lib -l curl


##########################################
# OOWe Test
##########################################
OBJS += test/main.o

BIN_TEST=test-oowe

##########################################
# Rules Test
##########################################
all : $(BIN_TEST)

%.o : %.cpp
	@echo "[CXX] $^"
	@$(CXX) $(CXX_FLAGS) $(INCS) -c -o $@ $^

$(BIN_TEST) : $(OBJS)
	@echo "[LD]  $@"
	@$(LD) $(LD_FLAGS) $(LIBS) -o $@ $^


clean :
	@echo "[RM]  $(OBJS)"
	@rm -fr $(OBJS)
	@echo "[RM]  $(BIN_TEST)"
	@rm -fr $(BIN_TEST)


test : $(BIN_TEST)
	$(BIN_TEST) http://www.google.com http://www.yahoo.fr

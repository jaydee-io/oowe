CXX_FLAGS= -std=c++11 -Wall -Werror
CXX=clang++
LD=clang++

##########################################
# OOWe Library
##########################################
OBJS += src/String.o
OBJS += src/Session.o
OBJS += src/HttpSession.o
OBJS += src/HttpPost.o

INCS += -I include
INCS += -I $(CURL_DIR)/include

LIBS += -L $(CURL_DIR)/lib -l curl


##########################################
# OOWe Test
##########################################
OBJS_TEST_FUNC  += test/main.o
OBJS_TEST_TIMES += test/times.o

BIN_TEST_FUNC  = test-oowe
BIN_TEST_TIMES = test-times

SITES_GAFA   = http://www.google.com http://www.amazon.com http://www.facebook.com http://www.apple.com
SITES_OTHERS = http://www.twitter.com http://www.yahoo.com
NB_RUN       = 1000

##########################################
# Rules Test
##########################################
all : $(BIN_TEST_FUNC) $(BIN_TEST_TIMES)

%.o : %.cpp
	@echo "[CXX] $^"
	@$(CXX) $(CXX_FLAGS) $(INCS) -c -o $@ $^

$(BIN_TEST_FUNC) : $(OBJS) $(OBJS_TEST_FUNC)
	@echo "[LD]  $@"
	@$(LD) $(LD_FLAGS) $(LIBS) -o $@ $^

$(BIN_TEST_TIMES) : $(OBJS) $(OBJS_TEST_TIMES)
	@echo "[LD]  $@"
	@$(LD) $(LD_FLAGS) $(LIBS) -o $@ $^


clean :
	@echo "[RM]  $(OBJS)"
	@rm -fr      $(OBJS)
	@echo "[RM]  $(OBJS_TEST_FUNC)"
	@rm -fr      $(OBJS_TEST_FUNC)
	@echo "[RM]  $(OBJS_TEST_TIMES)"
	@rm -fr      $(OBJS_TEST_TIMES)
	@echo "[RM]  $(BIN_TEST_FUNC)"
	@rm -fr      $(BIN_TEST_FUNC)
	@echo "[RM]  $(BIN_TEST_TIMES)"
	@rm -fr      $(BIN_TEST_TIMES)


test : $(BIN_TEST_FUNC) $(BIN_TEST_TIMES)
	$(BIN_TEST_FUNC)            $(SITES_GAFA) $(SITES_OTHERS)
	$(BIN_TEST_TIMES) $(NB_RUN) $(SITES_GAFA) $(SITES_OTHERS)

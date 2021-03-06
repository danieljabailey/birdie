Program = birdie

Objects  = birdie_types.o birdie_token_stream.o
Objects += birdie_funcs_add.o birdie_funcs_sub.o birdie_funcs_mul.o birdie_funcs_div.o 
Objects += birdie_funcs_booland.o birdie_funcs_boolor.o birdie_funcs_boolxor.o 
Objects += birdie_funcs.o
Objects += birdie_funcs_bool_util.o 
Objects += birdie_control.o birdie_builtin.o birdie_stackman.o
Objects += birdie_opts.o birdie_exits.o
Objects += debug_firehose.o

#GlobalConfig = -ggdb -DGLOBAL_DEBUG -Wreturn-type
#GlobalConfig = -ggdb -Wreturn-type
GlobalConfig = -ggdb -Wreturn-type -DYYDEBUG=1

FixedConfig = 

LinkExtra = -lm -lreadline

all: ${Program}

${Program}.tab.c: ${Program}.y
		bison -d $^
		
lex.yy.c: ${Program}.l ${Program}.tab.h
	flex $<

infoheader.h: gen_infoheader.sh
	./$^ > $@

-include $(Objects:.o=.d)

%.d: %.c
	@set -e; rm -f $@; \
	gcc -M $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o: %.c
	gcc -c $< -o $@ $(GlobalConfig) $(FixedConfig)

${Program}: ${Program}.tab.c lex.yy.c ${Objects}
	gcc -o $@ $^ -lfl $(GlobalConfig) $(LinkExtra)

${Program}.output: ${Program}.y
	bison -d --report=state $^

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.s
	rm -f *.d
	rm -f *.ii
	rm -f *.tab.* *.yy.* ${Program}
	rm -f ${Program}.output

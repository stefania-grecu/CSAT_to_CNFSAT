build:
	gcc -g -o main main.c -lm
run:
	./main $(INPUT) $(OUTPUT)
clean:
	rm tema2

all: asfhackpatch_do asfhackpatch_undo


asfhackpatch_do:
	gcc -o asfhackpatch_do asfhackpatch_do.c


asfhackpatch_undo:
	gcc -o asfhackpatch_undo asfhackpatch_undo.c


clean:
	rm asfhackpatch_do asfhackpatch_undo




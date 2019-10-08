
** A List of all files your're submitting and their purpose **

--- total 11 files(i dont make main.h)

1. main.c : main function to call all of the other function for excuting

2. readfile.c : readfile and save into data linked list

3. process.c : it takes linked list then processing the drawing, moving, plotting and coloring action also save output data into output linked list

4. writefile.c : it takes output linked list then write into the file with specific form.

5. effects.c : it is provided by university, and it draws line and has many the other function for terminal screen

6. readfile.h : readfile.c's header file, it includes data linked list stucts and predeclaration readfile.c's function 

7. writefile.h : writefile.c's header file, it includes output linked list structs and predeclaration writefile.c's function

8. process.h : process.c's header file, it has predeclaration process.c's function

9. effects.h : it is provided by university, it has type define plotFunc for plotting the line and predeclaration effects.c's function

** A statement of how much of the assignment you completed; spcifically;**

- i guess it works very well any of required by assignment note
- i tested some cases, all of the case are working very well.

** A list of issues reported by valgrind, if any, along with any other bugs or defects you know about, if any.** 

- i run with valgrind and it doesnt have memory leak, no error from context.

** A statement of which computer you tested your code on**

- it tested on my own laptop with gcc compiler in linux(fedora)
- because curtin college any computer not provide gcc compiler 

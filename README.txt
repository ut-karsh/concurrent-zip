Hello!
--
I have used gdb (correctly) and print commands to test this code a million times. 
I have had parts of the code running individually, but as soon as I put it together, I seg fault in the very first parallel instruction itself, and gdb wouldn't tell mewhy.
--
This is an account of the failure of 90+ hours of continuous wakefulness (and more prior time).
When beginning the build, I used my-zip as my base and significantly modified it to be more efficient. Tested it.
Then wrote an algorithm for the multithreading program, outlined its steps.
After that, I started writing the code in steps, beginning from consolidating the data and creating the threads. Then I worked on trying to make the code truly parallel and using as less locks as possible.
I used locks whenever a global variable was either initialized, or could have been initialized.
I stored the values of globals in their local counterparts so that critical values do not change during  a run when a global is changed.
Created two three dimensional arrays to store the compressed output. Two arrays , one for ints and other for chars-> faster storage, more convenient.
The three dimensions are: 1. File Number. 2. Run/Iteration Number. 3. Compression number.
It may seem complicated but in fact is very easy to implement.
After completing runs, I access both arrays and output them accordingly.
I have had parts of the code running individually, but as soon as I put it together, I seg fault in the very first parallel instruction itself, and gdb wouldn't tell mewhy.

## [Projects](http://a-nikolaev.github.io/fp/projects/)

### Description

In here lies the project description as provided by our professor, [Alexey Nikolaev](https://github.com/a-nikolaev), as well as the our project presentation ([PDF](<./OGalaga Presentation.pdf>)|[Google Slides](http://bit.ly/ogalaga)) and [technical write-up](null).

In the second half of the semester, you will work on a project, which will replace regular weekly homeworks.

### Timeline:

- March 7 - **Choose a project topic**.
- March 14 - **First draft of your project proposal**: 3-5 paragraphs description of the project goal, scope, and implementation plan.
- March 28 - **Finalized project proposal**: Have a proof of concept. Create a Github repository and send me its link.
- May - **Project presentations** (quick presentation in class and a 2-3 page report).

The topic for your project must be discussed and aproved before you start working on it.

Projects can be either individual or implemented by two-person teams.

**Github repo:**
You will have to create a Github repository for your project, so your progress can be tracked.
Update the repository regularly, at least once a week.
For two-person projects, both participants should commit to the project (coordinate your work well).
If your repository remains dormant until the last few days, when you suddenly upload everything,
you may get a low grade for the project.

**On borrowing code:**
If you use someone else's code, it must be clearly stated in the project README, what code is yours, and what code is borrowed.
Try to keep borrowed code in separate modules (separate files) from your own code to make distinction clear.

**Functional style:**
Your implementation should use only functional aspects of the language
(modules List, Map, Set, and String should be your main data structure, together with varaint types, tuples, and immutable records).
You still can use libraries that work in the imperative fashion with hidden mutable state, however your own code should stay functional or mostly functional.

<center><img src="https://i.imgur.com/pA52XaK.png" /></center>

### Possible project topics

- Check out the [Awesome-OCaml](https://github.com/ocaml-community/awesome-ocaml) collection, if you like any of those
  projects or libraries, it can help you pick an idea for your program.
- Think what application you would like to develop. It does not have to be big and complex, but should be non-trivial enough.
  It should pose a challenge, but you have to have a relatively concrete implementation plan of how to solve it.

Some possible specific topics (if you don't have any particular idea in mind):

- Huffman coding for file compression (there is a functional tree construction algorithm at its core).
- Markdown to HTML converter (also can design your own markup language).
- Scientific simulation or mathematical modeling for Physics/Chemistry/Biology.
- Generating diagrams or images using vector graphics library Cairo (package [cairo2](https://github.com/Chris00/ocaml-cairo)).
  Take a look at [Context Free Art](https://www.contextfreeart.org/index.html) for examples of recursive declarative images.
- Simple data plotting software similar to [gnuplot](http://www.gnuplot.info/) or [matplotlib](https://matplotlib.org/),
  using library Cairo (package [cairo2](https://github.com/Chris00/ocaml-cairo)) for output.
- Declarative terminal graphics with package [notty](https://github.com/pqwy/notty).
- Simple video game (using package [tsdl](https://erratique.ch/software/tsdl) or
  built-in module [Graphics](https://caml.inria.fr/pub/docs/manual-ocaml/libref/Graphics.html)).
- GUI application ([Tcl/Tk](http://labltk.forge.ocamlcore.org/index.html) or [GTK](http://lablgtk.forge.ocamlcore.org/)).
- Declarative reactive programming with package [react](https://github.com/dbuenzli/react).
- Theory: Lambda calculus, combinator calculus, concatenative languages (e.g. Joy).
- Interpreter for a simple programming language (if you take the programming languages class), implemented using parsers such as Menhir and Opal.
- Pretty printer for a C-style language (similar to [astyle](http://astyle.sourceforge.net/)).
- Your own OCaml syntax extension (ppx).
- Foreign Function Interface (interfacing with C code).
- Web application using Sinatra-like framework [opium](https://github.com/rgrinberg/opium).
- Property fuzzing for OCaml programs with package [crowbar](https://github.com/stedolan/crowbar).
- Lightweight concurrency in OCaml using [Lwt](https://github.com/ocsigen/lwt).
- A Unix utility such as `tree` or `grep`. Should implement command-line flags and options. Use modules Sys and Unix.
- Of course, you are encouraged to come up with your own topic, that would be even better.

### Grading criteria

- Good function decomposition: Design your functions well, good helper functions make code easy to write and read
- Not using imperative constructs
- Meeting your stated project goals: It is okay to backtrack sometimes, but reaching your initial goals is always a plus.
- Complexity of the chosen topic: You can be a bit ambitious.
- Topic originality: An interesting original project can be useful to other people.
- Regularity of the code updates on Github: New commits at least once a week is a good pace.
- Presentation and report quality

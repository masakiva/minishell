# minishell

This is a project for the 42 curriculum, in which you are asked to implement a basic shell command interpreter in C. The shell should be able to execute basic commands, handle redirections and pipelines, and support built-in commands such as `cd`, `echo`, and `env`.  
The behavior of `minishell` is based on the `bash` shell.

I did this project with [abenoit](https://github.com/42-abenoit) (here is the original repo https://framagit.org/abenoit/minishell), and it was my first group project at 42.

### Getting Started

To get started with the project, you can clone this repository and navigate to the project directory:

```
git clone https://github.com/masakiva/minishell.git
cd minishell
```

You can then build the project:

```
make
```

This will compile the source code for the `minishell` program and create an executable file named `minishell`.

You can then run the `minishell` program and start using the shell:

```
./minishell
```

### Project Requirements

The project is designed to test your ability to write complex programs in C, and to work effectively in a team environment. You will need to implement the following features:

- Basic command execution.
- Handling of single and double quotes.
- Handling of environment variables (e.g. `$HOME`, `$PWD`, `%?`, etc.).
- Redirections (`>`, `>>` and `<`).
- Pipelines (e.g. `ls | grep foo`).
- Built-in commands (`cd`, `echo`, `pwd`, `env`, `export`, `unset`, and `exit`).
- Proper handling of signals (`CTRL-C`, `CTRL-\` and `CTRL-D`).

You will also need to ensure that your code is well-organized, properly commented, and conforms to the 42 coding standards.

### Resources

Here are some resources that you may find useful when working on this project:

- [The Linux Programming Interface](http://man7.org/tlpi/)
- [Advanced Programming in the UNIX Environment](https://www.apuebook.com/)
- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/html_node/index.html)

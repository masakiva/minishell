#!/bin/bash

./minishell < tester_input > output___minishell
bash --posix < tester_input > output___bash
diff -s output___minishell output___bash

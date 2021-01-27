#!/bin/bash

./minishell < tester_input > output___minishell
bash < tester_input > output___bash
diff -s output___minishell output___bash

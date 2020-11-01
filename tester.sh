#!/bin/bash

./minishell < tester_input > output__minishell
bash < tester_input > output___bash
diff -s output__minishell output___bash

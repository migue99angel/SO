#!/bin/bash

ps -ef > `date +%Y-%j-%T`
echo Mi PID = $$ >> `date +%Y-%j-%T`
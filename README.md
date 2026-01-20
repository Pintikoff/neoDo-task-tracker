NeoDo - Command Line Task Tracker
 __   __     ______     ______     _____     ______  
/\ "-.\ \   /\  ___\   /\  __ \   /\  __-.  /\  __ \   
\ \ \-.  \  \ \  __\   \ \ \/\ \  \ \ \/\ \ \ \ \/\ \  
 \ \_\\"\_\  \ \_____\  \ \_____\  \ \____-  \ \_____\ 
  \/_/ \/_/   \/_____/   \/_____/   \/____/   \/_____/
  
A simple and efficient command-line task tracker written in C. Manage your tasks directly from the terminal with persistent JSON storage.
Project inspired by roadmap.sh Task Tracker Project
Features

✅ Add, update, and delete tasks
📝 Track task status (todo, in-progress, done)
💾 Persistent storage using JSON format
🔍 Filter tasks by status
🎨 Clean command-line interface

Installation
Prerequisites

GCC compiler
C standard library

Build from source
bashgit clone https://github.com/yourusername/neodo.git
cd neodo
gcc -o neoDo main.c
Optional: Add to PATH
To use neoDo from anywhere in your terminal:
bash# Linux/macOS
sudo cp neoDo /usr/local/bin/

# Or add to your PATH
export PATH=$PATH:/path/to/neodo
Usage
Add a Task
bashneoDo -a "Buy groceries"
neoDo -a "Finish homework"
List Tasks
bash# List all tasks
neoDo -l

# List tasks by status
neoDo -l todo
neoDo -l in-progress
neoDo -l done

# Or use status IDs
neoDo -l 1  # todo
neoDo -l 2  # in-progress
neoDo -l 3  # done
Update Task Status
bash# Using status name
neoDo -m 1 in-progress
neoDo -m 2 done

# Using status ID
neoDo -m 1 2  # Changes task #1 to in-progress
Update Task Content
bashneoDo -u 1 "Buy milk and bread"
Delete a Task
bashneoDo -d 3  # Deletes task with ID 3
Clear All Tasks
bashneoDo -clear
Help
bashneoDo -h
Task Status Options
Status IDStatus NameDescription1todoTask to be done2in-progressTask in progress3doneCompleted task
Data Storage
Tasks are stored in neoDo.json in the current directory. The file is automatically created on first use.
Example structure:
json[
	{
		"taskId": 1,
		"content": "Buy groceries",
		"status": "todo"
	},
	{
		"taskId": 2,
		"content": "Finish homework",
		"status": "in-progress"
	}
]
Examples
bash# Add a new task
neoDo -a "Finish homework"

# List all tasks
neoDo -l

# Change task #1 to in-progress
neoDo -m 1 in-progress

# List all completed tasks
neoDo -l done

# Update task content
neoDo -u 1 "Finish math homework"

# Delete task #3
neoDo -d 3
Error Handling
The application includes error handling for:

Invalid task IDs
Invalid status values
Empty or whitespace-only content
File I/O errors

Contributing
Contributions are welcome! Please feel free to submit a Pull Request.
License
This project is open source and available under the MIT License.
Acknowledgments

Project idea from roadmap.sh
Built with C standard library

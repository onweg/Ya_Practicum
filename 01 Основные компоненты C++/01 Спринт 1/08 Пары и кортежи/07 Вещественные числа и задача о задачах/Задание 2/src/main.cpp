#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
	  NEW,          // новая
	  IN_PROGRESS,  // в разработке
	  TESTING,      // на тестировании
	  DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
	  // Получить статистику по статусам задач конкретного разработчика
	  const TasksInfo& GetPersonTasksInfo(const string& person) const {
		return person_to_task_info.at(person);
	  }
	  
	  // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
	  void AddNewTask(const string& person) {
		person_to_task_info[person][TaskStatus::NEW]++;
	  }
	  
	  // Обновить статусы по данному количеству задач конкретного разработчика,
	  // подробности см. ниже
	  tuple<TasksInfo, TasksInfo> PerformPersonTasks(
						const string& person, int task_count){
			TasksInfo updated_tasks, untouched_tasks;
			TaskStatus status = TaskStatus::NEW;
			while (status != TaskStatus::DONE){
				if (task_count) {
					int d = min(task_count, person_to_task_info.at(person).at(status));
					task_count -= d;
					updated_tasks[static_cast<TaskStatus>(static_cast<int>(status) + 1)] = d;
					person_to_task_info[person][status] -= d;
				}
				if (!task_count) {
					untouched_tasks[status] = person_to_task_info[person][status];
				}
				status = static_cast<TaskStatus>(static_cast<int>(status) + 1);
			}
			status = TaskStatus::NEW;
			for (TaskStatus status = TaskStatus::NEW; status <= TaskStatus::DONE; status=static_cast<TaskStatus>(static_cast<int>(status) + 1)){
				person_to_task_info[person][status] += updated_tasks[status];
			}
			return make_tuple(updated_tasks, untouched_tasks);
		}

private:
	// я должен хранить map по имени и количеством задач со статусами
	// так я смогу легко добавлять новую задачу
	// по имени смогу получить статистику задач
	// обновить статусы по имени персоны 
	map<string, TasksInfo> person_to_task_info;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
        ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
        ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
        ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

int main() {
    TeamTasks tasks;
    tasks.AddNewTask("Ilia");
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan");
    }
    cout << "Ilia's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
    cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));
  
    TasksInfo updated_tasks, untouched_tasks;
  
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);
  
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);
}


	
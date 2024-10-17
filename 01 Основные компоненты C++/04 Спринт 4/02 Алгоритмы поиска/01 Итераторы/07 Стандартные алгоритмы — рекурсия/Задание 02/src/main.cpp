#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

class Tower {
public:
    // конструктор и метод SetDisks нужны, чтобы правильно создать башни
    Tower(int disks_num) {
        FillTower(disks_num);
    }

    int GetDisksNum() const {
        return disks_.size();
    }

    void SetDisks(int disks_num) {
        FillTower(disks_num);
    }

    void PrintTower() const {
        for (const int disk : disks_)
            cout << disk << " "s;
        cout << endl;
    }

    // добавляем диск на верх собственной башни
    // обратите внимание на исключение, которое выбрасывается этим методом
    void AddToTop(int disk) {
        int top_disk_num = disks_.size() - 1;
        if (0 != disks_.size() && disk >= disks_[top_disk_num]) {
            throw invalid_argument("Невозможно поместить большой диск на маленький"s);
        } else {
            // допишите этот метод и используйте его в вашем решении
            disks_.push_back(disk);
        }
    }

    vector<int> GetDisks() {
        return disks_;
    }

    // вы можете дописывать необходимые для вашего решения методы

    int GetTopDisk() {
        int top_disk_num = disks_.size() - 1;
        if (top_disk_num < 0) {
            throw invalid_argument("Невозможно взять диск так как башня пустая"s);
        } else {
            int value_disk = disks_[top_disk_num];
            disks_.pop_back();
            return value_disk;
        }
    }

private:
    vector<int> disks_;

    // используем приватный метод FillTower, чтобы избежать дубликации кода
    void FillTower(int disks_num) {
        for (int i = disks_num; i > 0; i--) {
            disks_.push_back(i);
        }
    }
};

bool operator==(Tower lhs_tower, Tower rhs_tower) {
    auto lhs = lhs_tower.GetDisks();
    auto rhs = rhs_tower.GetDisks();
    if (lhs.size() != rhs.size()) return false;
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) 
            return false;
    }
    return true;
}

void MoveHanoi(vector<Tower>& towers, Tower& sender_tower, int count_disk, Tower& recipient_tower, Tower& buffer_tower) {
    if (count_disk == 1) {
        recipient_tower.AddToTop(sender_tower.GetTopDisk());

        int send_id, recip_id;
        if (sender_tower == towers[0]) send_id = 1;
        else if (sender_tower == towers[1]) send_id = 2;
        else send_id = 3;
        if (recipient_tower == towers[0]) recip_id = 1;
        else if (recipient_tower == towers[1]) recip_id = 2;
        else recip_id = 3;

        cout << send_id << " -> "s << recip_id << endl;
        
        return ;
    }
    MoveHanoi(towers, sender_tower, count_disk - 1, buffer_tower, recipient_tower);
    MoveHanoi(towers, sender_tower, 1, recipient_tower, buffer_tower);
    MoveHanoi(towers, buffer_tower, count_disk - 1, recipient_tower, sender_tower);
}

void SolveHanoi(vector<Tower>& towers) {
    int disks_num = towers[0].GetDisksNum();

    // допишите функцию, чтобы на towers[0] было 0 дисков,
    // на towers[1] 0 дисков,
    // и на towers[2] было disks_num дисков

    //        от куда    сколько    куда       буфер башня
    MoveHanoi(towers, towers[0], disks_num, towers[2], towers[1]);

}

void PrintTowers(const vector<Tower>& towers) {
    cout << "первая башня: "s;
    towers[0].PrintTower();
    cout << "вторая башня: "s;
    towers[1].PrintTower();
    cout << "третья башня: "s;
    towers[2].PrintTower();
}

int main() {
    int n;
    cin >> n;
    Tower first_tower(n);
    Tower second_tower(0);
    Tower third_tower(0);
    vector<Tower> towers = {first_tower, second_tower, third_tower};
    cout << "Башни до \n"s;
    PrintTowers(towers);
    cout << endl;
    SolveHanoi(towers);
    cout << "\nБашни после \n"s;
    PrintTowers(towers);
    return 0;
}
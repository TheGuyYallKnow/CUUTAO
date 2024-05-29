/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C=0, S, W, SW };

class Character;
class Watson;
class Sherlock;
class Criminal;

class BaseItem;
class BaseBag;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class MovingObject;
class Position;
class SherlockBag;
class WatsonBag;
class TestStudyInPink;

class MapElement {
friend class TestStudyInPink;
protected:
    ElementType type;

public:
    MapElement(ElementType in_type) : type(in_type) {}
    virtual ~MapElement() {}

    virtual ElementType getType() const {
        return type;
    }
};

class Path : public MapElement {
friend class TestStudyInPink;
public:
    Path();
};

class Wall : public MapElement {
friend class TestStudyInPink;
public:
    Wall();
};

class FakeWall : public MapElement {
friend class TestStudyInPink;
private:
    int req_exp;
public:
    FakeWall(int in_req_exp);
    int getReqExp();
};

class Map {
friend class TestStudyInPink;
private:
    int num_rows;
    int num_cols;
    MapElement*** map;
public:
    bool isValid(const Position& pos, MovingObject* mv_obj) const;
    Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls);
    ~Map();
};

class Position {
friend class TestStudyInPink;
private:
public:
    static const Position npos;
    int r, c;
    Position(int r = 0, int c = 0) : r(r), c(c) {}
    Position(const string& str_pos) {
        stringstream ss(str_pos);
        char ch;
        ss >> ch >> r >> ch >> c >> ch;
    }

    int getCol() const {
        return c;
    }

    void setRow(int r) {
        this->r = r;
    }

    int getRow() const {
        return r;
    }

    string str() const {
        return "(" + std::to_string(r) + "," + std::to_string(c) + ")";
    }

    void setCol(int column) {
        this->c = column;
    }

    bool isEqual(const Position& pos) const {
        return pos.r == r && pos.c == c;
    }
    bool isEqual(int in_r, int in_c) const {
        return in_r == r && in_c == c;
    }

    int manhattanDistance(const Position& other) const {
        return abs(this->r - other.getRow()) + abs(this->c - other.getCol());
    }
    // Friendly Solutions found by any chance
    bool operator!=(const Position& other) const {
        return r != other.r || c != other.c;
    }

    friend ostream& operator<<(ostream& os, const Position& pos) {
        os << "(" << pos.r << "," << pos.c << ")";
        return os;
    }
    
    friend class TestStudyInPink;
};

class MovingObject {
friend class TestStudyInPink;
protected:
    int index;
    Position pos;
    Map * map;
    string name;

public:
    MovingObject(int index, const Position& pos, Map* map, const string& name = "")
        : index(index), pos(pos), map(map), name(name) {}
    virtual ~MovingObject() {};
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const { return pos; }
    virtual void move() = 0;
    virtual string str() const = 0;
};
class Character : public MovingObject {
friend class TestStudyInPink;
public:
    Character(int in_index, const Position& in_pos, Map* in_map, string Flag);
    virtual ~Character() {}
};
class Sherlock : public Character {
friend class TestStudyInPink;
private:
    string moving_rule;
    int hp;
    int exp;
    int rule_index = 0;
    bool Used_ExcemptionCard = false;
    BaseBag *bag;
public:
    Sherlock(int in_index, const string & in_moving_rule, const Position & in_init_pos, Map * in_map, int in_init_hp, int in_init_exp);
    ~Sherlock();
    Position getCurrentPosition();
    Position getNextPosition();
    void SetExcemptionCard(bool Set);
    bool CheckExcemptionCard();
    void move() override;
    int getHp() const;
    int getExp() const;
    void setHp(double value);
    void setExp(double value);
    void setBag(BaseBag* Bag);
    BaseBag *getBag() const {return bag;};

    void CheckIfMeeting(RobotC *robotC, Criminal *criminal);
    void CheckIfMeeting(Watson *watson);
    void CheckIfMeeting(RobotS *robotS);
    void CheckIfMeeting(RobotW *robotW);
    void CheckIfMeeting(RobotSW *robotSW);
    void CheckIfMeeting(Criminal *criminal);

    string str() const override;
};
class Watson : public Character {
friend class TestStudyInPink;
private:
    string moving_rule;
    int hp,exp;
    int move_index = 0;
    bool Used_PassingCard = false;
    BaseBag *bag;
public:
    Watson(int in_index, const string & in_moving_rule, const Position & in_init_pos, Map * in_map, int in_init_hp, int in_init_exp);
    Position getCurrentPosition();
    void SetPassingCard(bool Set);
    bool CheckPassingCard();
    Position getNextPosition();
    virtual ~Watson();
    void move();
    int getHp() const;
    void setHp(double value);
    int getExp() const;
    void setExp(double value);

    void setBag(BaseBag* Bag);
    BaseBag *getBag() {return bag;};

    void CheckIfMeeting(RobotSW *robotSW);
    void CheckIfMeeting(RobotS *robotS);
    void CheckIfMeeting(Criminal *criminal);
    void CheckIfMeeting(RobotC *robotC,Criminal *criminal);
    void CheckIfMeeting(RobotW *robotW);
    void CheckIfMeeting(Sherlock *sherlock);
    
    string str() const;
};
class Criminal : public Character {
friend class TestStudyInPink;
private:
    Sherlock* sherlock;
    Watson* watson;
    bool IsCaught = false;
    string moving_rule;
    int rule_index = 0;
    int Counter = 0;
    Position PreviousPosition;
public:
    Criminal(int in_index, const Position & in_init_pos, Map * in_map, Sherlock * in_sherlock, Watson * in_watson);
    Position getCurrentPosition();
    Position getNextPosition();
    Position getPreviousPosition();
    void setCaught(bool value);
    virtual ~Criminal();
    void move();
    bool isCaught() const;
    int getCount();
    string str() const;
};
class ArrayMovingObject {
friend class TestStudyInPink;
private:
    MovingObject** arr_mv_objs;
    int count;
    int capacity;
public:
    ArrayMovingObject(int in_capacity);
    ~ArrayMovingObject();
    MovingObject* get(int index) {
        return arr_mv_objs[index];
    }
    bool isFull() const;
    bool add(MovingObject * mv_obj);
    int getCount() const;
    string str() const;
};
class Configuration {
friend class StudyPinkProgram;
friend class TestStudyInPink;
private:
    int getNumSteps() const;
    int map_num_rows;
    int map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position* arr_walls;
    int num_fake_walls;
    Position* arr_fake_walls;
    std::string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    std::string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps;
public:
    Configuration(const string & in_filepath);
    ~Configuration();
    string str() const;
};

// Robots
class Robot : public MovingObject {
friend class TestStudyInPink;
protected:
    RobotType robot_type;
    string Flag;
    BaseItem* item;
public:
    Robot(int in_index, const Position& in_init_pos, Map* in_map, string in_Flag);
    virtual ~Robot();
    virtual int getDistance() const {return 0;};
    void generateItem(int i, int j);
    void giveItem(BaseBag* Bag);
    string getstr();
};
class RobotC : public Robot {
friend class TestStudyInPink;
protected:
    Criminal* criminal;
public:
    RobotC(int in_index, const Position& in_init_pos, Map* in_map,Criminal* in_criminal);
    Position getNextPosition();
    int getDistance(Sherlock *sherlock);
    int getDistance(Watson *watson);
    void move();
    string str() const;
};
class RobotSW : public Robot {
friend class TestStudyInPink;
protected:
    Criminal* criminal;
    Sherlock* sherlock;
    Watson* watson;
public:
    RobotSW(int in_index, const Position& in_init_pos, Map* in_map, Criminal* in_criminal, Sherlock* in_sherlock, Watson* in_watson);
    Position getNextPosition();
    int getDistance() const;
    void move();
    string str() const;
};
class RobotS : public Robot {
friend class TestStudyInPink;
protected:
    Criminal* criminal;
    Sherlock* sherlock;
public:
    RobotS(int in_index, const Position& in_init_pos, Map* in_map, Criminal* in_criminal, Sherlock* in_sherlock);
    Position getNextPosition();
    int getDistance() const;
    void move();
    string str() const;
};
class RobotW : public Robot {
friend class TestStudyInPink;
protected:
    Criminal* criminal;
    Watson* watson;
public:
    RobotW(int in_index, const Position& in_init_pos, Map* in_map, Criminal* in_criminal, Watson* in_watson);
    Position getNextPosition();
    int getDistance() const;
    void move();
    string str() const;
};


// Items
class BaseItem {
friend class TestStudyInPink;
public:
    virtual bool canUse(Character* in_obj, Robot * in_robot) = 0;
    virtual void use(Character* in_obj, Robot * in_robot) = 0;
    virtual ItemType getType() const = 0;
    virtual string getName() const = 0;
};
class MagicBook : public BaseItem {
friend class TestStudyInPink;
public:
    bool canUse(Character* in_obj, Robot * in_robot);
    void use(Character* in_obj, Robot * in_robot);
    ItemType getType() const override;
    string getName() const override;
};
class EnergyDrink : public BaseItem {
friend class TestStudyInPink;
public:
    bool canUse(Character* in_obj, Robot * in_robot);
    void use(Character* in_obj, Robot * in_robot);
    ItemType getType() const override;
    string getName() const override;
};
class FirstAid : public BaseItem {
friend class TestStudyInPink;
public:
    bool canUse(Character* in_obj,Robot * in_robot);
    void use(Character* in_obj,Robot * in_robot);
    ItemType getType() const override;
    string getName() const override;
};
class ExcemptionCard : public BaseItem {
friend class TestStudyInPink;
public:
    bool canUse(Character* in_obj,Robot * in_robot);
    void use(Character* in_obj,Robot * in_robot);
    ItemType getType() const override;
    string getName() const override;
};
class PassingCard : public BaseItem {
friend class TestStudyInPink;
private:
    string challenge;
public:
    bool canUse(Character* in_obj,Robot * in_robot);
    void use(Character* in_obj,Robot * in_robot);
    PassingCard(const string& in_challenge) : challenge(in_challenge) {}
    ItemType getType() const override;
    string getName() const override;
};

// Basebag
class BaseBag {
    friend class TestStudyInPink;
protected:
    Character* obj; // Char
    BaseItem** ItemList; // Itemlist
    int Quantity; // Item Quantity
    int Capacity; // Capacity
public:
    BaseBag(Character* in_character, int in_capacity) : obj(in_character), Capacity(in_capacity), Quantity(0) {
        this->ItemList = new BaseItem*[Capacity];
        for (int i = 0; i < this->Capacity; ++i) {
            this->ItemList[i] = nullptr;
        }
    }
    virtual ~BaseBag() {
        delete[] ItemList;
    }
    virtual bool insert(BaseItem* in_item) = 0;
    virtual BaseItem* get(Robot* in_Robotto) = 0;
    virtual BaseItem* get(ItemType in_itemType) = 0;
    virtual string str() const = 0;
    int getitemCount() {return this->Quantity;};
    void setitemCount(int Val) {this->Quantity = Val;};
    virtual BaseItem* get() = 0;
    BaseItem** getItems();
};
class WatsonBag : public BaseBag {
    friend class TestStudyInPink;
    Watson* watson;
public:
    WatsonBag(Watson* watson) : BaseBag(dynamic_cast<Character*>(watson), 15) {};
    bool insert(BaseItem* item);
    BaseItem* get() override;
    BaseItem* get(Robot* Robotto) override;
    BaseItem* get(ItemType itemType) override;
    string str() const override;
};
class SherlockBag : public BaseBag {
    friend class TestStudyInPink;
    Sherlock* sherlock;
public:
    SherlockBag(Sherlock* sherlock) : BaseBag(dynamic_cast<Character*> (sherlock), 13) {};
    bool insert(BaseItem* item);
    BaseItem* get() override;
    BaseItem* get(Robot* Robotto) override;
    BaseItem* get(ItemType itemType) override;
    string str() const override;
};

// Final PRogrem
class StudyPinkProgram {
    friend class TestStudyInPink;
private:
BaseBag *watsonBag; // WatsonBag
    Map * map; // Map
    ArrayMovingObject * arr_mv_objs; // ArrayMVObjects
    BaseBag *sherlockBag; // SherlockBag
    Sherlock * sherlock; // Sherlock
    Watson * watson; // Watson
    Configuration * config; // Config
    Criminal * criminal; // Criminal
    RobotC *robotC; // RobotC
public:
    StudyPinkProgram(const string & config_file_path);
    ~StudyPinkProgram();
    bool isStop() const;
    Robot* GetRobot(Position Pos);
    void printResult() const {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Watson caught the criminal" << endl;
        }
        else {
            cout << "The criminal escaped" << endl;
        }
    }
    void printStep(int si, int i) const {
        cout << "Step: " << setw(4) << setfill('0') << si
            << "--"
            << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }
    void run(bool verbose);
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */

#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

/*
Missings:
FakeWall
*/

// Function stringToItemType chuyen string thanh ItemType
ItemType stringtoItemType(string Flag)
{
    if (Flag == "EXCEMPTION_CARD")
        return EXCEMPTION_CARD;
    else if (Flag == "FIRST_AID")
        return FIRST_AID;
    else if (Flag == "ENERGY_DRINK")
        return ENERGY_DRINK;
    else if (Flag == "MAGIC_BOOK")
        return MAGIC_BOOK;
    else
        return PASSING_CARD;
}
// Function FixedInt
int FixedInt(double Sent,int Min,int Max) {
    if (Sent > Max) {
        Sent = Max;
    } else if (Sent < Min) {
        Sent = 0;
    }
    Sent = ceil(Sent);
    return static_cast<int>(Sent);
}

// Initializiation
Path::Path() : MapElement(PATH) {}
Wall::Wall():MapElement(WALL) {};
FakeWall::FakeWall(int in_req_exp) : MapElement(FAKE_WALL), req_exp(in_req_exp) {
    if (in_req_exp < 0) {
        req_exp = 0;
    } else {
        req_exp = in_req_exp;
    }
}
int FakeWall::getReqExp() {
    return req_exp;
}

const Position Position::npos = {-1, -1};
Character::Character(int index, const Position& pos, Map* Map, string CharacterName)
    : MovingObject(index, pos, Map, CharacterName) {}
// Map
Map::Map(int Rows, int Cols, int Walls, Position* ArrWalls, int FakeWalls, Position* ArrFakeWalls) {
    num_rows = Rows;
    num_cols = Cols;

    map = new MapElement**[num_rows];
    for (int i = 0; i < num_rows; i++) {
        map[i] = new MapElement*[num_cols];
        for (int j = 0; j < num_cols; j++) {
            map[i][j] = new Path();
        }
    }
    // Dat Tuong Gia
    // Tuong Gia < Tuong
    for (int i = 0; i < FakeWalls; ++i) {
        int r = ArrFakeWalls[i].getRow();
        int c = ArrFakeWalls[i].getCol();
        delete map[r][c];
        map[r][c] = new FakeWall((r * 257 + c * 139 + 89) % 900 + 1);
    }
    // Dat Tuong
    for (int i = 0; i < Walls; i++) {
        int r = ArrWalls[i].getRow();
        int c = ArrWalls[i].getCol();
        delete map[r][c];
        map[r][c] = new Wall();
    }
}
bool Map::isValid(const Position& pos, MovingObject* mv_obj) const {
    if (pos.r < 0 || pos.r >= this->num_rows || pos.c < 0 || pos.c >= this->num_cols) {
        return false;
    }
    MapElement* element = map[pos.r][pos.c];
    if (element->getType() == ElementType::WALL) {
        return false;
    }
    // Kiểm tra xem đối tượng di chuyển là gì
    if (dynamic_cast<Sherlock*>(mv_obj)) {
        // La Sherlock
        return true;
    } else if (dynamic_cast<Watson*>(mv_obj)) {
        // La Watson
        if (map[pos.r][pos.c] != NULL) { // Kiem tra thu 2
            MapElement* element = map[pos.getRow()][pos.getCol()];
            FakeWall* fakeWall = dynamic_cast<FakeWall*>(element);
            if (fakeWall) {
                // La Fake Wall
                if (dynamic_cast<Watson*>(mv_obj)->getExp() <= fakeWall->getReqExp()) { 
                    // Du Exp!
                    return false;
                }
            }
        }
        return true;
    }
    return true;
}
Map::~Map() {
    for (int index = 0; index < num_rows; ++index) {
        for (int sideIndex = 0; sideIndex < num_cols; ++sideIndex) {
            delete map[index][sideIndex];
        }
        delete[] map[index];
    }
    delete[] map;
}

// Sharlock
Sherlock::Sherlock(int Fromindex, const string & Fromimoving_rule, const Position & Fromiinit_pos, Map * Fromimap, int Fromiinit_hp, int Fromiinit_exp):Character(Fromindex, Fromiinit_pos, Fromimap, "Sherlock"), moving_rule(Fromimoving_rule), hp(Fromiinit_hp), exp(Fromiinit_exp) {}
string Sherlock::str() const {
    ostringstream oss;
    oss << "Sherlock[index=" << index << ";pos=" << pos << ";moving_rule=" << moving_rule << "]";
    return oss.str();
}
Position Sherlock::getCurrentPosition(){
    return pos;
}
int Sherlock::getHp() const {
    return this->hp;
}
int Sherlock::getExp() const {
    return this->exp;
}
void Sherlock::setHp(double value) {
    this->hp = FixedInt(value,0,500);
}
void Sherlock::setExp(double value) {
    this->exp = FixedInt(value,0,900);
}
void Sherlock::setBag(BaseBag* Bag) {
    this->bag = Bag;
};
Position Sherlock::getNextPosition() {
    char move = moving_rule[rule_index];
    Position nextPOSITION = this->pos;
    switch (move) {
        case 'L':
            nextPOSITION.c -= 1;
            break;
        case 'R':
            nextPOSITION.c += 1;
            break;
        case 'U':
            nextPOSITION.r -= 1;
            break;
        case 'D':
            nextPOSITION.r += 1;
            break;
    }
    if (map->isValid(nextPOSITION, this)) {
        return nextPOSITION;
    } else {
        return Position::npos;
    }
}
Sherlock::~Sherlock() {};
void Sherlock::move() {
    if (this->exp == 0) return;
    Position nextPOSITION = getNextPosition();
    rule_index = (rule_index + 1) % moving_rule.size();
    if (nextPOSITION != Position::npos) {
        this->pos = nextPOSITION;
    }
}
bool Sherlock::CheckExcemptionCard() {
    return this->Used_ExcemptionCard;
}
void Sherlock::SetExcemptionCard(bool Set) {
    this->Used_ExcemptionCard = Set;
}

void Sherlock::CheckIfMeeting(RobotS *robotS) {
    Position CurrentPos = this->pos;
    Position RobotPos = robotS->getCurrentPosition();
    if (CurrentPos.isEqual(RobotPos)) {
        if (this->exp > 400) {
            robotS->giveItem(getBag());
        } else {
            if (CheckExcemptionCard()) {
                SetExcemptionCard(false);
            } else {
                double AfterCalc = getExp() * 0.9;
                setExp(AfterCalc);
            }
        }
        bag->get(nullptr);
    }
};
void Sherlock::CheckIfMeeting(RobotW *robotW) {
    Position CurrentPos = this->pos;
    Position RobotPos = robotW->getCurrentPosition();
    if (CurrentPos.isEqual(RobotPos)) {
        robotW->giveItem(getBag());
        bag->get(nullptr);
    }
};
void Sherlock::CheckIfMeeting(Criminal *criminal) {
    Position CurrentPos = this->pos;
    Position criminalPos = criminal->getCurrentPosition();
    if (CurrentPos.isEqual(criminalPos)) {
        criminal->setCaught(true);
    }
};
void Sherlock::CheckIfMeeting(Watson *watson) {
    Position CurrentPos = this->pos;
    Position watsonPos = watson->getCurrentPosition();
    if (CurrentPos.isEqual(watsonPos)) {
        int Quantity = bag->getitemCount();
        BaseItem** ItemList = bag->getItems();
        for (int i = 0; i < Quantity; ++i) {
            if (ItemList[i]->getType() == stringtoItemType("")) {
                BaseItem* PCard = bag->get(stringtoItemType(""));
                if (PCard) {
                    BaseBag* WatBag = watson->getBag();
                    WatBag->insert(PCard);
                }
            }
        }
    }
};
void Sherlock::CheckIfMeeting(RobotSW *robotSW) {
    Position CurrentPos = this->pos;
    Position RobotPos = robotSW->getCurrentPosition();
    if (CurrentPos.isEqual(RobotPos)) {
        if (getExp() > 300 && getHp() > 335) {
            robotSW->giveItem(getBag());
        } else {
            if (CheckExcemptionCard()) {
                SetExcemptionCard(false);
            } else {
                double newExp = getExp() * 0.85;
                double newHp = getHp() * 0.85;
                setExp(newExp);
                setHp(newHp);
            }
        }
        bag->get(nullptr);
    }
};
void Sherlock::CheckIfMeeting(RobotC *robotC, Criminal *criminal) {
    Position CurrentPos = this->pos;
    Position RobotPos = robotC->getCurrentPosition();
    if (CurrentPos.isEqual(RobotPos)) {
        if (getExp() > 500) {
            criminal->setCaught(true);
        } else {
            robotC->giveItem(getBag());
        }
        bag->get(nullptr);
    }
};

// Wattoson
Watson::Watson(int Fromindex, const std::string& Frommoving_rule, const Position& Frominit_pos, Map* Frommap, int Frominit_hp, int Frominit_exp):Character(Fromindex, Frominit_pos, Frommap, "Watson"), moving_rule(Frommoving_rule), hp(Frominit_hp), exp(Frominit_exp) {}
string Watson::str() const {
    std::ostringstream oss;
    oss << "Watson[index=" << index << ";pos=" << pos << ";moving_rule=" << moving_rule << "]";
    return oss.str();
}
Position Watson::getNextPosition() {
    char move = this->moving_rule[move_index];
    Position nextPOSITION = this->pos;
    switch (move) {
        case 'L':
            nextPOSITION.c -= 1;
            break;
        case 'R':
            nextPOSITION.c += 1;
            break;
        case 'U':
            nextPOSITION.r -= 1;
            break;
        case 'D':
            nextPOSITION.r += 1;
            break;
    }
    if (this->map->isValid(nextPOSITION,this)) {
        return nextPOSITION;
    } else {
        return Position::npos;
    }
}
Position Watson::getCurrentPosition(){
    return this->pos;
}
Watson::~Watson() {};
void Watson::move() {
    if (getExp() == 0) return;
    Position nextPOSITION = getNextPosition();
    this->move_index = (this->move_index + 1) % this->moving_rule.size();
    if (nextPOSITION != Position::npos) {
        this->pos = nextPOSITION;
    }
}
void Watson::setHp(double value) {
    this->hp = FixedInt(value,0,500);
}
void Watson::setExp(double value) {
    this->exp = FixedInt(value,0,900);
}
void Watson::setBag(BaseBag* Bag) {
    this->bag = Bag;
};
int Watson::getHp() const {
    return this->hp;
}
int Watson::getExp() const {
    return this->exp;
}
void Watson::SetPassingCard(bool Set) {
    this->Used_PassingCard = Set;
}
bool Watson::CheckPassingCard() {
    return Used_PassingCard;
}
void Watson::CheckIfMeeting(Criminal *criminal) {
    Position CurrentPos = getCurrentPosition();
    Position criminalPos = criminal->getCurrentPosition();
    if (CurrentPos.isEqual(criminalPos)) {
        criminal->setCaught(true);
    }
};
void Watson::CheckIfMeeting(Sherlock *sherlock) {
    Position CurrentPos = this->pos;
    Position sherPos = sherlock->getCurrentPosition();
    if (CurrentPos.isEqual(sherPos)) {
        int Quantity = bag->getitemCount();
        BaseItem** ItemList = bag->getItems();
        for (int i = 0; i < Quantity; ++i) {
            if (ItemList[i]->getType() == stringtoItemType("EXCEMPTION_CARD")) {
                BaseItem* ExCard = bag->get(stringtoItemType("EXCEMPTION_CARD"));
                if (ExCard) {
                    BaseBag* SherBag = sherlock->getBag();
                    SherBag->insert(ExCard);
                }
            }
        }
    }
};
void Watson::CheckIfMeeting(RobotSW *robotSW) {
    Position CurrentPos = this->pos;
    Position RobotPos = robotSW->getCurrentPosition();
    if (CurrentPos.isEqual(RobotPos)) {
        if (getHp() > 165 && getExp() > 600) {
            robotSW->giveItem(getBag());
        } else {
            if (CheckPassingCard()) {
                SetPassingCard(false);
            } else {
                double newExp = getExp() * 0.85;
                double newHp = getHp() * 0.85;
                setExp(newExp);
                setHp(newHp);;
            }
        }
        this->bag->get(nullptr);
    }
};
void Watson::CheckIfMeeting(RobotC *robotC, Criminal *criminal) {
    Position CurrentPos = this->pos;
    Position RobotPos = robotC->getCurrentPosition();
    if (CurrentPos.isEqual(RobotPos)) {
        robotC->giveItem(getBag());
        this->bag->get(nullptr);
    }
};
void Watson::CheckIfMeeting(RobotS *robotS) {
    Position CurrentPos = this->pos;
    Position RobotPos = robotS->getCurrentPosition();
    if (CurrentPos.isEqual(RobotPos)) {
        return;
    }
};
void Watson::CheckIfMeeting(RobotW *robotW) {
    Position CurrentPos = this->pos;
    Position RobotPos = robotW->getCurrentPosition();
    if (CurrentPos.isEqual(RobotPos)) {
        if (this->hp > 350) {
            robotW->giveItem(getBag());
        } else {
            if (CheckPassingCard()) {
                SetPassingCard(false);
            } else {
                double newHP = getHp() * 0.95;
                setHp(newHP);
            }
        }
        this->bag->get(nullptr);
    }
};

// Robber
Criminal::Criminal(int Fromindex, const Position& Frominit_pos, Map* Frommap, Sherlock* Fromsherlock, Watson* Fromwatson):Character(Fromindex, Frominit_pos, Frommap, "Criminal"), sherlock(Fromsherlock), watson(Fromwatson), PreviousPosition(Position::npos) {}
Position Criminal::getNextPosition(){
    Position nextPOSITION = Position::npos;
    int FurthestDISTANCE = -1;
    pair<int, int> PseudoDIRECTIONS[] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
    for (const auto& dir : PseudoDIRECTIONS) {
        Position newPOSITION(this->pos.r + dir.first, this->pos.c + dir.second);
        if (map->isValid(newPOSITION, this)) {
            int DistanceToSHERLOCK = newPOSITION.manhattanDistance(sherlock->getCurrentPosition());
            int DistanceToWATSON = newPOSITION.manhattanDistance(watson->getCurrentPosition());
            int distance = DistanceToSHERLOCK + DistanceToWATSON;
            if (distance > FurthestDISTANCE) {
                FurthestDISTANCE = distance;
                nextPOSITION = newPOSITION;
            }
        }
    }
    return nextPOSITION;
}
Position Criminal::getCurrentPosition(){
    return pos;
}
Position Criminal::getPreviousPosition(){
    return PreviousPosition;
}
void Criminal::move() {
    Position nextPOSITION = getNextPosition();
    if (nextPOSITION != Position::npos) {
        if (!nextPOSITION.isEqual(pos)) {
            Counter += 1;
        }
        PreviousPosition = pos;
        pos = nextPOSITION;
    }
}
bool Criminal::isCaught() const {
    return IsCaught;
}
void Criminal::setCaught(bool value) {
    this->IsCaught = value;
}
Criminal::~Criminal() {};
string Criminal::str() const {
    std::ostringstream oss;
    oss << "Criminal[index=" << index << ";pos=" << pos << "]";
    return oss.str();
}
int Criminal::getCount() {
    return Counter;
}

// ArrayMovingObjects
ArrayMovingObject::ArrayMovingObject(int Fromcapacity) : capacity(Fromcapacity), count(0) {
    this->arr_mv_objs = new MovingObject*[capacity];
}
int ArrayMovingObject::getCount() const {
    return this->count;
}
string ArrayMovingObject::str() const {
    std::ostringstream oss;
    oss << "ArrayMovingObject[count=" << this->count << ";capacity=" << this->capacity;
    if (this->count > 0) { oss << ";";};
    for (int i = 0; i < this->count; ++i) {
        oss << this->arr_mv_objs[i]->str();
        if (i < this->count-1) oss << ";";
    }
    oss << "]";
    return oss.str();
}
bool ArrayMovingObject::isFull() const {
    return this->count == this->capacity;
}
ArrayMovingObject::~ArrayMovingObject() {}
bool ArrayMovingObject::add(MovingObject* mv_obj) {
    if (this->isFull()) {
        return false;
    }
    this->arr_mv_objs[count++] = mv_obj;
    return true;
}

// Robot
void Robot::generateItem(int x, int y) {
    int p = x * y;
    int s = 0;
    while (p > 0) {
        s += p % 10;
        p /= 10;
    }
    while (s > 9) {
        int temp = s;
        s = 0;
        while (temp > 0) {
            s += temp % 10;
            temp /= 10;
        }
    }

    if (s <= 1) {
        this->item = new MagicBook();
    } else if (s <= 3) {
        this->item = new EnergyDrink();
    } else if (s <= 5) {
        this->item = new FirstAid();
    } else if (s <= 7) {
        this->item = new ExcemptionCard();
    } else {
        int t = (x * 11 + y) % 4;
        string challenge;
        switch (t) {
            case 0: challenge = "RobotS"; break;
            case 1: challenge = "RobotC"; break;
            case 2: challenge = "RobotSW"; break;
            case 3: challenge = "all"; break;
        }
        this->item = new PassingCard(challenge);
    }
}
Robot::~Robot() {};
Robot::Robot(int Fromin_index, const Position& Fromin_pos, Map* Fromin_map, string FromFlag):MovingObject(Fromin_index, Fromin_pos, Fromin_map, FromFlag), Flag(FromFlag) {generateItem(getCurrentPosition().r,getCurrentPosition().c);}
string Robot::getstr(){
    return this->Flag;
}
void Robot::giveItem(BaseBag* TargetBag) {
    TargetBag->insert(this->item);
}

// Variants
RobotC::RobotC(int Fromindex, const Position& Frominit_pos, Map* Frommap, Criminal* Fromcriminal)
    : Robot(Fromindex, Frominit_pos, Frommap, "RobotC"), criminal(Fromcriminal) {}
int RobotC::getDistance(Watson* watson) {
    if (watson) {
        return getCurrentPosition().manhattanDistance(watson->getCurrentPosition());
    }
    return 0;
}
void RobotC::move() {
    Position nextPOSITION = getNextPosition();
    if (nextPOSITION != Position::npos) {
        pos = nextPOSITION;
    }
}
int RobotC::getDistance(Sherlock* sherlock) {
   if (sherlock) {
        return getCurrentPosition().manhattanDistance(sherlock->getCurrentPosition());
    }
    return 0;
}
string RobotC::str() const {
    ostringstream oss;
    oss << "Robot[pos=" << getCurrentPosition() << ";type=C;dist=]";
    return oss.str();
}
Position RobotC::getNextPosition() {
    Position nextPOSITION = this->criminal->getPreviousPosition();
    if (nextPOSITION.isEqual(Position::npos)) {return Position::npos;};
    if (map->isValid(nextPOSITION, this)) {
        return nextPOSITION;
    } else {
        return Position::npos;
    }
}

RobotS::RobotS(int Fromindex, const Position& Frominit_pos, Map* Frommap, Criminal* Fromcriminal, Sherlock* Fromsherlock):Robot(Fromindex, Frominit_pos, Frommap, "RobotS"), criminal(Fromcriminal), sherlock(Fromsherlock) {}
int RobotS::getDistance() const {
    Position SherlockPos = this->sherlock->getCurrentPosition();
    int distance = getCurrentPosition().manhattanDistance(SherlockPos);
    return distance;
}
string RobotS::str() const {
    int distance = getDistance();
    ostringstream oss;
    oss << "Robot[pos=" << getCurrentPosition() << ";type=S;dist="<< distance << "]";
    return oss.str();
}
void RobotS::move() {
    Position nextPOSITION = getNextPosition();
    if (nextPOSITION != Position::npos) {
        this->pos = nextPOSITION;
    }
}
Position RobotS::getNextPosition() {
    Position nextPOSITION = Position::npos;

    pair<int, int> PseudoDIRECTIONS[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    int FurthestDISTANCE = 10000;
    for (const auto& dir : PseudoDIRECTIONS) {
        Position newPOSITION(getCurrentPosition().r + dir.first, getCurrentPosition().c + dir.second);
        if (map->isValid(newPOSITION, this)) {
            int DistanceToSHERLOCK = newPOSITION.manhattanDistance(this->sherlock->getCurrentPosition());
            if (DistanceToSHERLOCK < FurthestDISTANCE) {
                FurthestDISTANCE = DistanceToSHERLOCK;
                nextPOSITION = newPOSITION;
            }
        } else {
        }
    }
    
    if (this->map->isValid(nextPOSITION, this)) {
        return nextPOSITION;
    } else {
        return Position::npos;
    }
}

RobotW::RobotW(int Fromindex, const Position& Frominit_pos, Map* Frommap, Criminal* Fromcriminal, Watson* Fromwatson):Robot(Fromindex, Frominit_pos, Frommap, "RobotW"), criminal(Fromcriminal), watson(Fromwatson) {}
int RobotW::getDistance() const {
    Position WatsonPos = this->watson->getCurrentPosition();
    int distance = getCurrentPosition().manhattanDistance(WatsonPos);
    return distance;
}
string RobotW::str() const {
    int distance = getDistance();
    ostringstream oss;
    oss << "Robot[pos=" << getCurrentPosition() << ";type=W;dist="<< distance << "]";
    return oss.str();
}
void RobotW::move() {
    Position nextPOSITION = getNextPosition();
    if (nextPOSITION != Position::npos) {
        this->pos = nextPOSITION;
    }
}
Position RobotW::getNextPosition() {
    Position nextPOSITION = Position::npos;

    pair<int, int> PseudoDIRECTIONS[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    int FurthestDISTANCE = 10000;
    for (const auto& dir : PseudoDIRECTIONS) {
        Position newPOSITION(getCurrentPosition().r + dir.first, getCurrentPosition().c + dir.second);
        if (this->map->isValid(newPOSITION, this)) {
            int DistanceToWATSON = newPOSITION.manhattanDistance(this->watson->getCurrentPosition());
            if (DistanceToWATSON < FurthestDISTANCE) {
                FurthestDISTANCE = DistanceToWATSON;
                nextPOSITION = newPOSITION;
            }
        }
    }

    if (this->map->isValid(nextPOSITION, this)) {
        return nextPOSITION;
    } else {
        return Position::npos;
    }
}

RobotSW::RobotSW(int Fromindex, const Position& Frominit_pos, Map* Frommap, Criminal* Fromcriminal, Sherlock* Fromsherlock, Watson* Fromwatson):Robot(Fromindex, Frominit_pos, Frommap, "RobotSW"), criminal(Fromcriminal), sherlock(Fromsherlock), watson(Fromwatson) {}
string RobotSW::str() const {
    int distance = getDistance();
    ostringstream oss;
    oss << "Robot[pos=" << getCurrentPosition() << ";type=SW;dist=" << distance << "]";
    return oss.str();
}
void RobotSW::move() {
    Position nextPOSITION = getNextPosition();
    if (nextPOSITION != Position::npos) {
        this->pos = nextPOSITION;
    }
}
Position RobotSW::getNextPosition() {
    Position nextPOSITION = Position::npos;

    pair<int, int> PseudoDIRECTIONS[] = {{-2, 0}, {-1,1}, {0, 2}, {1, 1}, {2, 0}, {1, -1}, {0, -2}, {-1, -1}};
    int FurthestDISTANCE = 10000;
    for (const auto& dir : PseudoDIRECTIONS) {
        Position newPOSITION(getCurrentPosition().r + dir.first, getCurrentPosition().c + dir.second);
        if (this->map->isValid(newPOSITION, this)) {
            int DistanceToWATSON = newPOSITION.manhattanDistance(this->watson->getCurrentPosition());
            int DistanceToSHERLOCK = newPOSITION.manhattanDistance(this->sherlock->getCurrentPosition());
            int Distance = DistanceToSHERLOCK + DistanceToWATSON;
            if (Distance < FurthestDISTANCE) {
                FurthestDISTANCE = Distance;
                nextPOSITION = newPOSITION;
            }
        }
    }

    if (this->map->isValid(nextPOSITION, this)) {
        return nextPOSITION;
    } else {
        return Position::npos;
    }
}
int RobotSW::getDistance() const {
    Position WatsonPos = this->watson->getCurrentPosition();
    Position SherlockPos = this->sherlock->getCurrentPosition();
    int DistanceToWatson = getCurrentPosition().manhattanDistance(WatsonPos);
    int DistanceToSherlock = getCurrentPosition().manhattanDistance(SherlockPos);
    return DistanceToWatson + DistanceToSherlock;
}

// Config
Configuration::Configuration(const string & Path){
    ifstream file(Path);
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string FromKey;
        string FromValue;
        getline(iss, FromKey, '=');
        getline(iss, FromValue);
        if (FromKey == "MAP_NUM_ROWS") {
            this->map_num_rows = stoi(FromValue);
        } else if (FromKey == "MAP_NUM_COLS") {
            this->map_num_cols = stoi(FromValue);
        } else if (FromKey == "MAX_NUM_MOVING_OBJECTS") {
            this->max_num_moving_objects = stoi(FromValue);
        } else if (FromKey == "ARRAY_WALLS") {
            string positions = FromValue.substr(1, FromValue.size() - 2);
            istringstream iss(positions);
            string Possibilities;
            this->arr_walls = nullptr;
            this->num_walls = 0;
            for (char c : positions) {
                if (c == '(') {
                    this->num_walls += 1;
                }
            }
            if (this->num_walls > 0) {
                this->arr_walls = new Position[this->num_walls];
                int i = 0;
                while (getline(iss, Possibilities, ';')) {
                    int x, y;
                    sscanf(Possibilities.c_str(), "(%d,%d)", &x, &y);
                    this->arr_walls[i] = Position(x, y);
                    i++;
                }
            }
        } else if (FromKey == "ARRAY_FAKE_WALLS") {
            string positions = FromValue.substr(1, FromValue.size() - 2);
            istringstream iss(positions);
            string Possibilities;
            this->arr_fake_walls = nullptr;
            this->num_fake_walls = 0;
            for (char c : positions) {
                if (c == '(') {
                    this->num_fake_walls += 1;
                }
            }
            if (this->num_fake_walls > 0) {
            this->arr_fake_walls = new Position[this->num_fake_walls];
            int i = 0;
            while (getline(iss, Possibilities, ';')) {
                int x, y;
                sscanf(Possibilities.c_str(), "(%d,%d)", &x, &y);
                this->arr_fake_walls[i] = Position(x, y);
                i++;
            }
        }
        } else if (FromKey == "SHERLOCK_MOVING_RULE") {
            this->sherlock_moving_rule = FromValue;
        } else if (FromKey == "SHERLOCK_INIT_POS") {
            int x;
            int y;
            sscanf(FromValue.c_str(), "(%d,%d)", &x, &y);
            this->sherlock_init_pos = Position(x, y);
        } else if (FromKey == "SHERLOCK_INIT_HP") {
            this->sherlock_init_hp = stoi(FromValue);
        } else if (FromKey == "SHERLOCK_INIT_EXP") {
            this->sherlock_init_exp = stoi(FromValue);
        } else if (FromKey == "WATSON_MOVING_RULE") {
            this->watson_moving_rule = FromValue;
        } else if (FromKey == "WATSON_INIT_POS") {
            int x;
            int y;
            sscanf(FromValue.c_str(), "(%d,%d)", &x, &y);
            this->watson_init_pos = Position(x, y);
        } else if (FromKey == "WATSON_INIT_HP") {
            this->watson_init_hp = stoi(FromValue);
        } else if (FromKey == "WATSON_INIT_EXP") {
            this->watson_init_exp = stoi(FromValue);
        } else if (FromKey == "CRIMINAL_INIT_POS") {
            int x;
            int y;
            sscanf(FromValue.c_str(), "(%d,%d)", &x, &y);
            this->criminal_init_pos = Position(x, y);
        } else if (FromKey == "NUM_STEPS") {
            this->num_steps = stoi(FromValue);
        }
    }
    file.close();
}
Configuration::~Configuration(){
    delete[] arr_walls;
    delete[] arr_fake_walls;
}
int Configuration::getNumSteps() const {
    return this->num_steps;
}
string Configuration::str() const {
    ostringstream oss;
    oss << "Configuration[\n"
        << "MAP_NUM_ROWS=" << this->map_num_rows << "\n"
        << "MAP_NUM_COLS=" << this->map_num_cols << "\n"
        << "MAX_NUM_MOVING_OBJECTS=" << this->max_num_moving_objects << "\n"
        << "NUM_WALLS=" << this->num_walls << "\n"
        << "ARRAY_WALLS=[";
    for (int i = 0; i < this->num_walls; ++i) {
        oss << "(" << this->arr_walls[i].r << "," << this->arr_walls[i].c << ")";
        if (i != this->num_walls - 1) {
            oss << ";";
        }
    }
    oss << "]\n"
        << "NUM_FAKE_WALLS=" << this->num_fake_walls << "\n"
        << "ARRAY_FAKE_WALLS=[";
    for (int i = 0; i < this->num_fake_walls; ++i) {
        oss << "(" << this->arr_fake_walls[i].r << "," << this->arr_fake_walls[i].c << ")";
        if (i != this->num_fake_walls - 1) {
            oss << ";";
        }
    }
    oss << "]\n"
        << "SHERLOCK_MOVING_RULE=" << this->sherlock_moving_rule << "\n"
        << "SHERLOCK_INIT_POS=(" << this->sherlock_init_pos.r << "," << this->sherlock_init_pos.c << ")\n"
        << "SHERLOCK_INIT_HP=" << this->sherlock_init_hp << "\n"
        << "SHERLOCK_INIT_EXP=" << this->sherlock_init_exp << "\n"
        << "WATSON_MOVING_RULE=" << this->watson_moving_rule << "\n"
        << "WATSON_INIT_POS=(" << this->watson_init_pos.r << "," << this->watson_init_pos.c << ")\n"
        << "WATSON_INIT_HP=" << this->watson_init_hp << "\n"
        << "WATSON_INIT_EXP=" << this->watson_init_exp << "\n"
        << "CRIMINAL_INIT_POS=(" << this->criminal_init_pos.r << "," << this->criminal_init_pos.c << ")\n"
        << "NUM_STEPS=" << this->num_steps << "\n"
        << "]";
    return oss.str();
}

// Basic ItemList in program
ItemType ExcemptionCard::getType() const {
    return EXCEMPTION_CARD;
}
bool ExcemptionCard::canUse(Character* obj,Robot * robot) {
    Sherlock* sherlock = dynamic_cast<Sherlock*>(obj);
    if (robot != nullptr && sherlock && sherlock->getHp() % 2 == 1) {
        return true;
    }
    return false;
}
string ExcemptionCard::getName() const {
    return "ExcemptionCard";
}
void ExcemptionCard::use(Character* obj,Robot * robot) {
    Sherlock* sherlock = dynamic_cast<Sherlock*>(obj);
    if (sherlock) {
        sherlock->SetExcemptionCard(false);
    }
};

string MagicBook::getName() const {
    return "MagicBook";
}
bool MagicBook::canUse(Character* obj,Robot * robot) {
    Sherlock* sherlock = dynamic_cast<Sherlock*>(obj);
    Watson* watson = dynamic_cast<Watson*>(obj);
    if (sherlock) {
        if (sherlock->getExp() > 0&&sherlock->getExp() <= 350) return true;
    } else if (watson) {
        if (watson->getExp() > 0&&watson->getExp() <= 350) return true;
    }
    return false;
}
ItemType MagicBook::getType() const {
    return MAGIC_BOOK;
}
void MagicBook::use(Character* obj,Robot * robot) {
    Sherlock* sherlock = dynamic_cast<Sherlock*>(obj);
    Watson* watson = dynamic_cast<Watson*>(obj);
    if (sherlock) {
        double AfterCalc = sherlock->getExp();
        AfterCalc = AfterCalc + AfterCalc * 0.25;
        sherlock->setExp(AfterCalc);
    } else if (watson) {
        double AfterCalc = watson->getExp();
        AfterCalc = AfterCalc + AfterCalc * 0.25;
        watson->setExp(AfterCalc);
    }
};

string FirstAid::getName() const {
    return "FirstAid";
}
bool FirstAid::canUse(Character* obj,Robot * robot) {
    Sherlock* sherlock = dynamic_cast<Sherlock*>(obj);
    Watson* watson = dynamic_cast<Watson*>(obj);
    if (sherlock) {
        if (sherlock->getExp() <= 350||sherlock->getHp() <= 100) return true;
    } else if (watson) {
        if (watson->getExp() <= 350||watson->getHp() <= 100) return true;
    }
    return false;
}
ItemType FirstAid::getType() const {
    return FIRST_AID;
}
void FirstAid::use(Character* obj,Robot * robot) {
    Sherlock* sherlock = dynamic_cast<Sherlock*>(obj);
    Watson* watson = dynamic_cast<Watson*>(obj);
    if (sherlock) {
        double AfterCalc = sherlock->getHp();
        AfterCalc = AfterCalc + AfterCalc * 0.5;
        sherlock->setHp(AfterCalc);
    } else if (watson) {
        double AfterCalc = watson->getHp();
        AfterCalc = AfterCalc + AfterCalc * 0.5;
        watson->setHp(AfterCalc);
    }
};

string EnergyDrink::getName() const {
    return "EnergyDrink";
}
bool EnergyDrink::canUse(Character* obj,Robot * robot) {
    Sherlock* sherlock = dynamic_cast<Sherlock*>(obj);
    Watson* watson = dynamic_cast<Watson*>(obj);
     if (sherlock) {
        if (sherlock->getHp() <= 100) return true;
    } else if (watson) {
        if (watson->getHp() <= 100) return true;
    }
    return false;
}
ItemType EnergyDrink::getType() const {
    return ENERGY_DRINK;
}
void EnergyDrink::use(Character* obj,Robot * robot) {
    Sherlock* sherlock = dynamic_cast<Sherlock*>(obj);
    Watson* watson = dynamic_cast<Watson*>(obj);

    if (sherlock) {
        double AfterCalc = sherlock->getHp();
        AfterCalc = AfterCalc + AfterCalc * 0.2;
        sherlock->setHp(AfterCalc);
    } else if (watson) {
        double AfterCalc = watson->getHp();
        AfterCalc = AfterCalc + AfterCalc * 0.2;
        watson->setHp(AfterCalc);
    }
};

string PassingCard::getName() const {
    return "PassingCard";
}
bool PassingCard::canUse(Character* obj,Robot * robot) {
    Watson* watson = dynamic_cast<Watson*>(obj);
    if (watson && robot != nullptr && watson->getHp() % 2 == 0) {
        return true;
    }
    return false;
}
ItemType PassingCard::getType() const {
    return PASSING_CARD;
}
void PassingCard::use(Character* obj,Robot * robot) {
    Watson* watson = dynamic_cast<Watson*>(obj);

    if (watson && robot != nullptr) {
        if (challenge == "all") {
            watson->SetPassingCard(true);
        } else {
            if (challenge == robot->getstr()) {
                watson->SetPassingCard(true);
            } else {
                double newEXP = watson->getExp() - 50;
                watson->setExp(newEXP);
            }
        }
    }
};

// Basebag
BaseItem** BaseBag::getItems() {
    return this->ItemList;
};

// BAG USERS
bool SherlockBag::insert(BaseItem* Insertion) {
    if (this->Quantity < this->Capacity) {
        for (int i = this->Quantity; i > 0; --i) {
            this->ItemList[i] = this->ItemList[i - 1];
        }
        this->ItemList[0] = Insertion;
        this->Quantity += 1;
        return true;
    }
    return false;
}
string SherlockBag::str() const {
    string result = "Bag[count=" + to_string(this->Quantity) + ";";
    for (int i = 0; i < this->Quantity; ++i) {
        if (i > 0) {
            result += ",";
        }
        result += (ItemList[i]->getName());
    }
    result += "]";
    return result;
}
BaseItem* SherlockBag::get(Robot* Type) {
    if (this->Quantity > 0) {
        for (int i = 0; i < this->Quantity; ++i) {
            BaseItem* QueuedItem = this->ItemList[i];
            if (Type != nullptr) {
                if (QueuedItem->canUse(this->obj,Type)) {
                    QueuedItem->use(this->obj,Type);
                    for (int i = 0; i < this->Quantity - 1; ++i) {
                        this->ItemList[i] = this->ItemList[i + 1];
                    }
                    this->Quantity -= 1;
                    return QueuedItem;
                }
            } else {
                if (QueuedItem->getType() != EXCEMPTION_CARD) {
                    if (QueuedItem->canUse(this->obj,Type)) {
                        QueuedItem->use(this->obj,Type);

                        for (int SideIndex = i; SideIndex < this->Quantity - 1; ++SideIndex) {
                            this->ItemList[SideIndex] = this->ItemList[SideIndex + 1];
                        }
                        this->Quantity -= 1;
                        return QueuedItem;
                    }
                }
            }
        }
    }
    return nullptr;
}
BaseItem* SherlockBag::get(ItemType Type) {
    for (int i = 0; i < this->Quantity; ++i) {
        if (this->ItemList[i]->getType() == Type) {
            BaseItem* temp = this->ItemList[0];
            this->ItemList[0] = this->ItemList[i];
            this->ItemList[i] = temp;
            
            BaseItem* QueuedItem = this->ItemList[0];
            if (QueuedItem->canUse(this->obj,nullptr)) {
                QueuedItem->use(this->obj,nullptr);
            }
            for (int SideIndex = i; SideIndex < this->Quantity - 1; ++SideIndex) {
                ItemList[SideIndex] = ItemList[SideIndex + 1];
            }
            this->Quantity -= 1;
            return QueuedItem;
        }
    }
    return nullptr;
}
BaseItem* SherlockBag::get() {
    return get(nullptr);
}
bool WatsonBag::insert(BaseItem* Insertion) {
    if (this->Quantity < this->Capacity) {
        for (int i = this->Quantity; i > 0; --i) {
            this->ItemList[i] = this->ItemList[i - 1];
        }
        this->ItemList[0] = Insertion;
        this->Quantity += 1;
        return true;
    }
    return false;
}
string WatsonBag::str() const {
    string result = "Bag[count=" + to_string(this->Quantity) + ";";
    for (int i = 0; i < this->Quantity; ++i) {
        if (i > 0) {
            result += ",";
        }
        result += (this->ItemList[i]->getName());
    }
    result += "]";
    return result;
}
BaseItem* WatsonBag::get(Robot* Type) {
    if (this->Quantity > 0) {
        for (int i = 0; i < this->Quantity; ++i) {
            BaseItem* QueuedItem = this->ItemList[i];
            if (Type != nullptr) {
                if (QueuedItem->canUse(this->obj,Type)) {
                    QueuedItem->use(this->obj,Type);
                    for (int SideIndex = i; SideIndex < this->Quantity - 1; ++SideIndex) {
                        this->ItemList[SideIndex] = this->ItemList[SideIndex + 1];
                    }
                    this->Quantity -= 1;
                    return QueuedItem;
                }
            } else {
                if (QueuedItem->getType() != PASSING_CARD) {
                    if (QueuedItem->canUse(obj,Type)) {
                        QueuedItem->use(this->obj,Type);
                        for (int i = 0; i < this->Quantity - 1; ++i) {
                            this->ItemList[i] = this->ItemList[i + 1];
                        }
                        this->Quantity -=1 ;
                        return QueuedItem;
                    }
                }
            }
        }
    }
    return nullptr;
}
BaseItem* WatsonBag::get(ItemType Type) {
    for (int i = 0; i < this->Quantity; ++i) {
        if (this->ItemList[i]->getType() == Type) {
            BaseItem* temp = this->ItemList[0];
            this->ItemList[0] = this->ItemList[i];
            this->ItemList[i] = temp;

            BaseItem* QueuedItem = this->ItemList[0];
            if (QueuedItem->canUse(this->obj,nullptr)) {
                QueuedItem->use(this->obj,nullptr);
            }
            for (int SideIndex = i; SideIndex < this->Quantity - 1; ++SideIndex) {
                ItemList[SideIndex] = ItemList[SideIndex + 1];
            }
            this->Quantity -= 1;
            return QueuedItem;
        }
    }
    return nullptr;
}
BaseItem* WatsonBag::get() {
    return get(nullptr);
}

// Program
StudyPinkProgram::StudyPinkProgram(const string & config_file_path){
    this->config = new Configuration(config_file_path);

    int Inserted_num_walls = this->config->num_walls;
    int Inserted_in_num_rows = this->config->map_num_rows;
    int Inserted_num_fake_walls = this->config->num_fake_walls;
    int Inserted_in_num_cols = this->config->map_num_cols;
    int Inserted_sherlock_init_exp = this->config->sherlock_init_exp;
    int Inserted_watson_init_exp = this->config->watson_init_exp;
    int Inserted_sherlock_init_hp = this->config->sherlock_init_hp;
    int Inserted_watson_init_hp = this->config->watson_init_hp;
    int Inserted_max_num_moving_objects = this->config->max_num_moving_objects;
    string Inserted_moving_rules_wat = this->config->watson_moving_rule;
    Position Inserted_init_pos_criminal = this->config->criminal_init_pos;
    string Inserted_moving_rules_sher = this->config->sherlock_moving_rule;
    Position *Inserted_arr_fake_walls = this->config->arr_fake_walls;
    Position Inserted_init_pos_watson = this->config->watson_init_pos;
    Position *Inserted_arr_walls = this->config->arr_walls;
    Position Inserted_init_pos_sher = this->config->sherlock_init_pos;

    this->robotC = nullptr;
    this->map = new Map(Inserted_in_num_rows, Inserted_in_num_cols, Inserted_num_walls, Inserted_arr_walls, Inserted_num_fake_walls, Inserted_arr_fake_walls);
    this->sherlock = new Sherlock(1, Inserted_moving_rules_sher, Inserted_init_pos_sher, this->map, Inserted_sherlock_init_hp, Inserted_sherlock_init_exp);
    this->sherlockBag = new SherlockBag(this->sherlock);
    this->watson = new Watson(2, Inserted_moving_rules_wat, Inserted_init_pos_watson, this->map, Inserted_watson_init_hp, Inserted_watson_init_exp);
    this->watsonBag = new WatsonBag(this->watson);
    this->criminal = new Criminal(0, Inserted_init_pos_criminal, this->map, this->sherlock, this->watson);
    BaseBag *PSEUDOSherBag = sherlockBag;
    this->sherlock->setBag(PSEUDOSherBag);
    BaseBag *PSEUDOWatBag = watsonBag;
    this->watson->setBag(PSEUDOWatBag);

    this->arr_mv_objs = new ArrayMovingObject(Inserted_max_num_moving_objects);
    this->arr_mv_objs->add(criminal);
    this->arr_mv_objs->add(sherlock);
    this->arr_mv_objs->add(watson);
}
StudyPinkProgram::~StudyPinkProgram() {}
Robot* StudyPinkProgram::GetRobot(Position Pos) {
    if (!Pos.isEqual(Position::npos)) {
        for (int i = 0; i < this->arr_mv_objs->getCount(); ++i) {
            if (this->arr_mv_objs->get(i)->getCurrentPosition().isEqual(Pos)) {
                MovingObject* Gotobj = this->arr_mv_objs->get(i);
                if (dynamic_cast<Robot*>(Gotobj)) {
                    return dynamic_cast<Robot*>(Gotobj);
                }
            }
        }
    }
    return nullptr;
}
bool StudyPinkProgram::isStop() const {
    return this->watson->getHp() == 0 || this->criminal->isCaught() || this->sherlock->getHp() == 0;
}
void StudyPinkProgram::run(bool verbose) {
    for (int istep = 0; istep < this->config->getNumSteps(); ++istep) {
        if (isStop()) {
            printResult();
            break;
        }
        for (int i = 0; i < this->arr_mv_objs->getCount(); ++i) {
            MovingObject* Gotobj = this->arr_mv_objs->get(i);

            if (isStop()) {
                printResult();
                break;
            }

            if (dynamic_cast<Sherlock*>(Gotobj)) {
                Position nextPOSITION = this->sherlock->getNextPosition();
                Robot* Target = GetRobot(nextPOSITION);
                this->sherlockBag->get(Target);
            } else if (dynamic_cast<Watson*>(Gotobj)) {
                Position nextPOSITION = this->watson->getNextPosition();
                Robot* Target = GetRobot(nextPOSITION);
                this->watsonBag->get(Target);
            }
            Gotobj->move();

            // Casting Criminal
            Criminal* IsCriminal = dynamic_cast<Criminal*>(Gotobj);
            if (IsCriminal && this->criminal->getCount() % 3 == 0 && this->criminal->getCount() > 0 && !this->arr_mv_objs->isFull()) {
                Robot* robot = nullptr;
                if (this->robotC == nullptr) {
                    this->robotC = new RobotC(3, criminal->getPreviousPosition(), this->map, this->criminal);
                    this->arr_mv_objs->add(this->robotC);
                } else {
                    Position SpawnPosition = this->criminal->getPreviousPosition();
                    int DistanceToSherlock = SpawnPosition.manhattanDistance(this->sherlock->getCurrentPosition());
                    int DistanceToWatson = SpawnPosition.manhattanDistance(this->watson->getCurrentPosition());
                    if (DistanceToSherlock > DistanceToWatson) {
                        robot = new RobotW(3, this->criminal->getPreviousPosition(), this->map, this->criminal, this->watson);
                        this->arr_mv_objs->add(robot);
                    } else if (DistanceToWatson > DistanceToSherlock) {
                        robot = new RobotS(3, this->criminal->getPreviousPosition(), this->map, this->criminal, this->sherlock);
                        this->arr_mv_objs->add(robot);
                    } else if (DistanceToSherlock == DistanceToWatson) {
                        robot = new RobotSW(3, this->criminal->getPreviousPosition(), this->map, this->criminal, this->sherlock, this->watson);
                        this->arr_mv_objs->add(robot);
                    }
                }
            }

            // Hitting or not
            if (RobotC* IsRobotC = dynamic_cast<RobotC*>(Gotobj)) {
                this->watson->CheckIfMeeting(this->robotC,this->criminal);
                this->sherlock->CheckIfMeeting(this->robotC,this->criminal);
            } else if (RobotS* IsrobotS = dynamic_cast<RobotS*>(Gotobj)) {
                this->watson->CheckIfMeeting(IsrobotS);
                this->sherlock->CheckIfMeeting(IsrobotS);
            } else if (RobotW* IsRobotW = dynamic_cast<RobotW*>(Gotobj)) {
                this->watson->CheckIfMeeting(IsRobotW);
                this->sherlock->CheckIfMeeting(IsRobotW);
            } else if (RobotSW* IsrobotSW = dynamic_cast<RobotSW*>(Gotobj)) {
                this->watson->CheckIfMeeting(IsrobotSW);
                this->sherlock->CheckIfMeeting(IsrobotSW);
            } else if (Watson* Iswatsonto = dynamic_cast<Watson*>(Gotobj)) {
                this->watson->CheckIfMeeting(this->sherlock);
                this->watson->CheckIfMeeting(this->criminal);
                if (!isStop()) {
                    for (int Attempts = 0; Attempts < this->arr_mv_objs->getCount(); ++Attempts) {
                        MovingObject* Sideobj = this->arr_mv_objs->get(Attempts);
                        if (RobotC* SiderobotC = dynamic_cast<RobotC*>(Sideobj)) {
                            this->watson->CheckIfMeeting(this->robotC, this->criminal);
                        } else if (RobotS* SiderobotS = dynamic_cast<RobotS*>(Sideobj)) {
                            this->watson->CheckIfMeeting(SiderobotS);
                        } else if (RobotW* SiderobotW = dynamic_cast<RobotW*>(Sideobj)) {
                            this->watson->CheckIfMeeting(SiderobotW);
                        } else if (RobotSW* SiderobotSW = dynamic_cast<RobotSW*>(Sideobj)) {
                            this->watson->CheckIfMeeting(SiderobotSW);
                        }
                    }
                }
            } else if (Sherlock* Issherlockto = dynamic_cast<Sherlock*>(Gotobj)) {
                this->sherlock->CheckIfMeeting(this->criminal);
                this->sherlock->CheckIfMeeting(this->watson);
                if (!isStop()) {
                    for (int Attempts = 0; Attempts < arr_mv_objs->getCount(); ++Attempts) {
                        MovingObject* Sideobj = this->arr_mv_objs->get(Attempts);
                        if (RobotC* SiderobotC = dynamic_cast<RobotC*>(Sideobj)) {
                            this->sherlock->CheckIfMeeting(this->robotC, this->criminal);
                        } else if (RobotS* SiderobotS = dynamic_cast<RobotS*>(Sideobj)) {
                            this->sherlock->CheckIfMeeting(SiderobotS);
                        } else if (RobotW* SiderobotW = dynamic_cast<RobotW*>(Sideobj)) {
                            this->sherlock->CheckIfMeeting(SiderobotW);
                        } else if (RobotSW* SiderobotSW = dynamic_cast<RobotSW*>(Sideobj)) {
                            this->sherlock->CheckIfMeeting(SiderobotSW);
                        }
                    }
                }
            } else if (Criminal* Iscriminalto = dynamic_cast<Criminal*>(Gotobj)) {
                this->sherlock->CheckIfMeeting(this->criminal);
                this->watson->CheckIfMeeting(this->criminal);
            }

            if (verbose) {
                printStep(istep, i);
            }
        }
    }
    printResult();
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
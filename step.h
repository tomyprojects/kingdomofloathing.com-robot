#ifndef STEP_H
#define STEP_H

class Step
{
public:
    Step();

    typedef enum {WAIT = -1,
                  LOGIN,
                  SUBMIT,
                  IN_GAME,
                  UPDATE_CHARACTER_PANEL,
                  HEAL_1,
                  HEAL_2,
                  ADVENTURE_AGAIN,
                  ATTACK,
                  MEDICINE,
                  MEDICINE_2,
                  STEAL,
                  BUFF,
                  BUFF_1,
                  BUFF_2,
                  BUFF_3,
                  BUFF_4,
                  CHOICE2
                 } STEP;

    void wait();

    void next();

    int state();

    void go(int k);

private:

    int _step;
    int _state;
};

#endif // STEP_H

#include <Arduboy2.h>

Arduboy2 ab;

int scene;
int level;
int num1;
int num2;
int op;
int progress;
int answer;
int correct;
int length = 20;
int answers[20];
char formula[6];
char logs[21];
char operatars[3] = {'+', '-', 'x'};

enum scene {
  GAME_START,
  QUESTION_INIT,
  QUESTION_VIEW,
  QUESTION_ANSWER,
  GAME_RESULT,
};

void setup() {
  ab.begin();
  ab.clear();
  ab.setFrameRate(60);
  ab.initRandomSeed();
  level = 5;
  scene = GAME_START;
}

void loop() {
  if (!(ab.nextFrame())) return;

  ab.pollButtons();

  switch (scene) {
  case GAME_START:
    ab.clear();
    ab.setCursor(6, 0);
    ab.setTextSize(4);
    ab.print("nBack");
    ab.setTextSize(1);
    ab.setCursor(38, 48);
    ab.print(level);
    ab.print(" Back");
    ab.setCursor(6, 56);
    ab.print("answer first digit");
    if (ab.justPressed(UP_BUTTON)) {
      if (level < 10) level += 1;
    } else if (ab.justPressed(DOWN_BUTTON)) {
      if (level > 1) level -= 1;
    } else if (ab.justPressed(A_BUTTON)) {
      scene = QUESTION_INIT;
    }
    break;
  case QUESTION_INIT:
    ab.clear();
    memset(logs, '_', sizeof(logs));
    logs[20] = '\0';
    // ログ初期表示
    ab.setCursor(4, 34);
    ab.print(logs);
    answer = 0;
    progress = 0;
    correct = 0;
    scene = QUESTION_VIEW;
    break;
  case QUESTION_VIEW:
    ab.fillRect(0, 24, 128, 8, BLACK);
    if (progress < length) {
      num1 = random(0, 9);
      num2 = random(0, 9);
      op = random(0, 3);
      if (op == 0) {
        answers[progress] = (num1 + num2) % 10;
      } else if (op == 1) {
        answers[progress] = abs(num1 - num2) % 10;
      } else if (op == 2) {
        answers[progress] = (num1 * num2) % 10;
      }
      sprintf(formula, "%d %c %d", num1, operatars[op], num2);
      // 問題下矢印表示
      ab.setCursor(progress * 6 + 4, 24);
      ab.print(F("\x19"));
    } else {
      // 問題表示終了時
      memset(formula, ' ', sizeof(formula));
    }
    // 問題表示
    formula[5] = '\0';
    ab.setTextSize(2);
    ab.setCursor(32, 4);
    ab.print(formula);
    ab.setTextSize(1);
    if (progress >= level) {
      // 解答上矢印
      ab.setCursor((progress - level) * 6 + 4, 44);
      ab.print(F("\x18"));
      // 解答表示
      ab.setCursor((progress - level) * 6 + 4, 54);
      ab.print(answer);
    }
    scene = QUESTION_ANSWER;
    break;
  case QUESTION_ANSWER:
    if (progress >= level &&
        (ab.justPressed(UP_BUTTON) || ab.justPressed(DOWN_BUTTON))) {
      // 解答入力
      if (ab.justPressed(UP_BUTTON)) answer += 1;
      if (ab.justPressed(DOWN_BUTTON)) answer -= 1;
      if (answer > 9) answer = 0;
      if (answer < 0) answer = 9;
      ab.setCursor((progress - level) * 6 + 4, 54);
      ab.print(answer);
    } else if (ab.justPressed(A_BUTTON)) {
      // 解答決定
      if (progress >= level) {
        if (answers[progress - level] == answer) {
          logs[progress - level] = 'o';
          correct++;
        } else {
          logs[progress - level] = 'x';
        }
        ab.setCursor(4, 34);
        ab.print(logs);
      }
      ab.fillRect(0, 44, 128, 32, BLACK);
      progress += 1;
      scene = (length + level <= progress) ? GAME_RESULT : QUESTION_VIEW;
    }
    break;
  case GAME_RESULT:
    ab.setTextSize(2);
    ab.setCursor(48, 4);
    ab.print(correct * (100 / length));
    ab.print('%');
    if (ab.justPressed(A_BUTTON)) {
      scene = GAME_START;
    }
    break;
  }

  ab.display();
}

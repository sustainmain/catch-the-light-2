const int messagePin = 4;  // LCD display variable

#include <SoftwareSerial.h>
SoftwareSerial messenger = SoftwareSerial(255, messagePin);  // LCD

// I/O related
const int led[] = {
  12,
  11,
  10,
  9,
  8,
  7,
  6
};
const int animal[] = {
  13,
  3,
  2,
  1,
  0
};
const int buttonPin = 5;

// game-related
boolean lose = false;
int score = 0;
const String level[] = {
  "Sloth",
  "Cow",
  "Frog",
  "Monkey",
  "Cheetah"
};
const String ability[] = {
  "Slow",
  "Stampedes",
  "Hops",
  "Fast",
  "Has to rest"
};
boolean stampede = false;
boolean cheetahRest = false;
const int winScore = 5;

// light numbering
int currentLight = 1;
int lightDirection = 1;

// delay cycles
const int waitTimes[] = {
  450,
  420,
  600,
  150,
  80
};
const int stampedingCow = 50;    // waitTimes[1] for stampeding cow
const int restingCheetah = 400;  // waitTimes[4] for resting cheetah
int stampedeCycle = 0;
int stampedeEnd;
int cheetahRestCycle = 0;
int cheetahEndRest;
int cheetahStartRestCycle = 0;
int cheetahStartRest = 0;
int wait = waitTimes[score];
unsigned long prevMillis = 0;

void setup()
{
  for (int i = 0; i < 7; i++)  // set up LEDs
  {
    pinMode(led[i], OUTPUT);
  }
  for (int i = 0; i < 5; i++)  // set up animal LEDs
  {
    pinMode(animal[i], OUTPUT);
  }
  digitalWrite(animal[score], HIGH);
  pinMode(buttonPin, INPUT);
  pinMode(messagePin, OUTPUT);
  digitalWrite(messagePin, HIGH);
  messenger.begin(19200);
  delay(100);
  messenger.write(22);
  messenger.write(17);
  delay(10);  // DO NOT REMOVE
  messenger.write(216);  // select the 4th scale
  sendMessage(level[score]);
  messenger.write(13);
  messenger.print(ability[score]);
  delay(1000);
}



void loop()
{
  while (lose == false && score < winScore) // run while the player has neither won nor lost
  {
    if (digitalRead(buttonPin))
    {
      if (currentLight == 1 || currentLight == 7)
      {
        score++;
        for (int i = 0; i < 5; i++)
        {
          digitalWrite(animal[i], HIGH);
        }
        if (score != winScore)
        {
          delay(2000);
          for (int i = 0; i < 5; i++)
          {
            digitalWrite(animal[i], LOW);
          }
          digitalWrite(animal[score], HIGH);
          delay(750);
          wait = waitTimes[score];
          delay(1000);
          sendMessage("Level " + String(score + 1));
          messenger.write(13);
          messenger.print("Starting in...");
        }
        else
        {
          sendMessage("You win!");
        }
        delay(1000);

        setLights(LOW, LOW, LOW, LOW, LOW, LOW, LOW);
        delay(500);
        setLights(LOW, LOW, LOW, HIGH, LOW, LOW, LOW);
        delay(200);
        setLights(LOW, LOW, HIGH, LOW, HIGH, LOW, LOW);
        delay(200);
        setLights(LOW, HIGH, LOW, LOW, LOW, HIGH, LOW);
        delay(200);
        setLights(HIGH, LOW, LOW, LOW, LOW, LOW, HIGH);
        delay(200);
        setLights(LOW, HIGH, LOW, LOW, LOW, HIGH, LOW);
        delay(200);
        setLights(LOW, LOW, HIGH, LOW, HIGH, LOW, LOW);
        delay(200);
        setLights(LOW, LOW, LOW, HIGH, LOW, LOW, LOW);
        delay(250);

        if (score == 5)
        {
          break;
        }

        // timer to start next level
        messenger.write(148);
        messenger.print("4 seconds       ");
        setLights(HIGH, LOW, LOW, LOW, LOW, LOW, HIGH);
        delay(1000);
        messenger.write(148);
        messenger.print("3 seconds       ");
        setLights(LOW, HIGH, LOW, LOW, LOW, HIGH, LOW);
        delay(1000);
        messenger.write(148);
        messenger.print("2 seconds       ");
        setLights(LOW, LOW, HIGH, LOW, HIGH, LOW, LOW);
        delay(1000);
        messenger.write(148);
        messenger.print("1 second        ");
        setLights(LOW, LOW, LOW, HIGH, LOW, LOW, LOW);
        delay(1000);
        sendMessage(level[score]);
        messenger.write(13);
        messenger.print(ability[score]);
      }
      else
      {
        sendMessage("You lose!");
        delay(1000);
        lose = true;
        break;
      }
    }

    lightMover(score + 1);
  }
  delay(1000);

  for (int x = 0; x < 3; x++)
  {
    setLights(LOW, LOW, LOW, LOW, LOW, LOW, LOW);  // these lines run whether you won or lost
    delay(250);

    if (currentLight == 1 || currentLight == 7)    // if you won the game, this block runs then the other (read above comment) <--- 3 times
    {
      setLights(HIGH, LOW, LOW, LOW, LOW, LOW, HIGH);
    }
    else if (currentLight == 2)                    // this block and the rest run if you lost
    {
      setLights(LOW, HIGH, LOW, LOW, LOW, LOW, LOW);
    }
    else if (currentLight == 3)
    {
      setLights(LOW, LOW, HIGH, LOW, LOW, LOW, LOW);
    }
    else if (currentLight == 4)
    {
      setLights(LOW, LOW, LOW, HIGH, LOW, LOW, LOW);
    }
    else if (currentLight == 5)
    {
      setLights(LOW, LOW, LOW, LOW, HIGH, LOW, LOW);
    }
    else if (currentLight == 6)
    {
      setLights(LOW, LOW, LOW, LOW, LOW, HIGH, LOW);
    }
    delay(250);
  }

  if (currentLight == 1 || currentLight == 7)      // this runs if you won
  {
    for (int x = 0; x < 10; x++)
    {
      setLights(LOW, HIGH, HIGH, HIGH, HIGH, HIGH, LOW);
      delay(150);
      setLights(HIGH, LOW, LOW, LOW, LOW, LOW, HIGH);
      delay(150);
    }
  }
}

// set the state of each light
void setLights(int light1, int light2, int light3, int light4, int light5, int light6, int light7)
{
  digitalWrite(led[0], light1);
  digitalWrite(led[1], light2);
  digitalWrite(led[2], light3);
  digitalWrite(led[3], light4);
  digitalWrite(led[4], light5);
  digitalWrite(led[5], light6);
  digitalWrite(led[6], light7);
}

void lightMover(int level)
{
  if (millis() - prevMillis > wait)
  {
    prevMillis = millis();
    if (level == 1 || level == 2 || level == 4 || level == 5)
    {
      if (lightDirection == 1)
      {
        if (currentLight == 6)
        {
          lightDirection = -1;
        }
        currentLight++;
      }
      else if (lightDirection == -1)
      {
        if (currentLight == 2)
        {
          lightDirection = 1;
        }
        currentLight--;
      }

      setLights(LOW, LOW, LOW, LOW, LOW, LOW, LOW);
      digitalWrite(led[currentLight - 1], HIGH);

      if (level == 2)
      {
        if (stampede)
        {
          stampedeCycle++;  // this cycle counts before checking cycles (if statement below)
          if (stampedeCycle == stampedeEnd)
          {
            stampede = false;
            stampedeCycle = 0;
            wait = waitTimes[score];
          }
        }
        else if (random(1, 20) == 1)
        {
          stampede = true;
          stampedeEnd = (int)random(40, 140); // (milliseconds) Min: 40 * 50 (3 seconds), Max: 140 * 50 (7 seconds)
          wait = stampedingCow;
        }
      }
      else if (level == 5)
      {
        if (cheetahRest)
        {
          cheetahRestCycle++;  // this cycle counts before checking cycles (if statement below)
          if (cheetahRestCycle == cheetahEndRest)
          {
            cheetahRest = false;
            cheetahRestCycle = 0;
            cheetahEndRest = 0;
            cheetahStartRest = (int)random(7, 10);
            wait = waitTimes[score];
          }
        }
        else if (cheetahStartRestCycle == cheetahStartRest)
        {
          cheetahRest = true;
          cheetahStartRestCycle = 0;
          cheetahEndRest = (int)random(2, 4);
          wait = restingCheetah;
        }
        else
        {
          cheetahStartRestCycle++;
        }
      }
    }
    else if (level == 3)
    {
      currentLight = (int)random(1, 7);
      setLights(LOW, LOW, LOW, LOW, LOW, LOW, LOW);
      digitalWrite(led[currentLight - 1], HIGH);
    }
  }
}

void sendMessage(String message)
{
  messenger.write(12);  // Delete all text and go to (0, 0)
  delay(10);  // DO NOT REMOVE
  messenger.print(message);
  messenger.write(212);
  messenger.write(220);
}


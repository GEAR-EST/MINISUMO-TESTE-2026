class Motor {
  int pwm, pin1, pin2;

  public:
    void pinout(int pin_pwm, int IN1, int IN2){
      pwm = pin_pwm;
      pin1 = IN1;
      pin2 = IN2;

      pinMode(pwm, OUTPUT);
      pinMode(pin1, OUTPUT);
      pinMode(pin2, OUTPUT);
    }

    void write(int pot){
      int sent = 1;

      if (pot < 0) {
        pot = -pot;
        sent = -1;
      }

      analogWrite(pwm, pot);

      switch (sent){
      case 1:
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
        break;
      
      case -1:
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
        break;
      }
  }
};

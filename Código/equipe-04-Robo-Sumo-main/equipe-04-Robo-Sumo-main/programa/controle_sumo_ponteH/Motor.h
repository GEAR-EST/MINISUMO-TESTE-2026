class Motor {
    int pin1, pin2;
    
    public:
        void pinout(int IN1, int IN2){
            pin1 = IN1;
            pin2 = IN2;

            pinMode(pin1, OUTPUT);
            pinMode(pin2, OUTPUT);
        }

        void write(int sent){
            switch (sent){
            case 1:
                digitalWrite(pin1, HIGH);
                digitalWrite(pin2, LOW);
                break;
            
            case -1:
                digitalWrite(pin1, LOW);
                digitalWrite(pin2, HIGH);
                break;

            case 0:
                digitalWrite(pin1, LOW);
                digitalWrite(pin2, LOW);
                break;
            }
        }
};

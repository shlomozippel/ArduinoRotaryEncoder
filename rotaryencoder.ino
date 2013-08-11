class RotaryEncoder {
	public:
		int encoderPos;
		bool pressed;
		bool pressedThisFrame;
		bool releasedThisFrame;
		unsigned long pressTime;
		unsigned long lastRelease;

		RotaryEncoder(int pinCLK, int pinDT, int pinSW) {
			this->pinCLK = pinCLK;
			this->pinDT = pinDT;
			this->pinSW = pinSW;
		}

		void Setup() {
			pinMode(this->pinCLK, INPUT);
			pinMode(this->pinDT, INPUT);
			pinMode(this->pinSW, INPUT);

			// pull up switch pin
			digitalWrite(this->pinSW, HIGH);

			this->lastCLK = digitalRead(this->pinCLK);
			this->lastDT = digitalRead(this->pinDT);
		}

		void Update() {
			// update CLK pin
			this->currentCLK = digitalRead(this->pinCLK);
			if (this->currentCLK != this->lastCLK) {
				this->currentDT ^ this->lastCLK ? this->encoderPos++ : this->encoderPos--;
				this->lastCLK = this->currentCLK;
			}
	
			// update DT pin
			this->currentDT = digitalRead(this->pinDT);
			if (this->currentDT != this->lastDT) {
				this->currentDT ^ this->lastCLK ? this->encoderPos++ : this->encoderPos--;
				this->lastDT = this->currentDT;
			}

			// update push switch with debouncing
			bool currentPressed = digitalRead(this->pinSW) == LOW;
			// edge?
			if (currentPressed != this->pressed) {
				// release
				if (this->pressed) {
					this->lastRelease = millis();
					this->pressed = false;
					this->releasedThisFrame = true;
				// press
				} else {
					// simple debounce
					if (millis() - this->lastRelease > 50) {
						this->pressTime = millis();
						this->pressed = true;
						this->pressedThisFrame = true;
					}
				}
			// no edge
			} else {
				this->pressedThisFrame = false;
				this->releasedThisFrame = false;
			}
		}

	protected:
		int pinCLK;
		int pinDT;
		int pinSW;

		int currentCLK;
		int lastCLK;
		int currentDT;
		int lastDT;
};

RotaryEncoder rotenc(4,5,3);
int lastRotenc = 0;

void setup()
{
	rotenc.Setup();

	Serial.begin (115200);
	Serial.println("start");
}

void loop()
{
	rotenc.Update();

	if (lastRotenc != rotenc.encoderPos) {
		lastRotenc = rotenc.encoderPos;
		Serial.println(lastRotenc);
	}

	if (rotenc.pressedThisFrame) {
		Serial.println("Pressed");
	}

	if (rotenc.releasedThisFrame) {
		Serial.println("Released");
	}
}

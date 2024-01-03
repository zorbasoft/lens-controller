#pragma once
#define MICROSTEP_DELAY_ms 5
class StepperController
{
public:
	StepperController(uint16_t encA_CLK_pin, uint16_t encB_DT_pin,
		uint16_t step_A1_pin, uint16_t step_A2_pin, uint16_t step_B1_pin, uint16_t step_B2_pin) {
		encA_clk_pin = encA_CLK_pin;
		encB_dt_pin = encB_DT_pin;
		step_a1_pin = step_A1_pin;
		step_a2_pin = step_A2_pin;
		step_b1_pin = step_B1_pin;
		step_b2_pin = step_B2_pin;
	}

	void initialize() {
		pinMode(encA_clk_pin, INPUT);
		pinMode(encB_dt_pin, INPUT);
		pinMode(step_a1_pin, OUTPUT);
		pinMode(step_a2_pin, OUTPUT);
		pinMode(step_b1_pin, OUTPUT);
		pinMode(step_b2_pin, OUTPUT);
		enc_counter = 0;
		stepIndex = 0;
		encA_last_state = digitalRead(encA_clk_pin);
		set_step(0);
	}  

	void loop() {
		encA_state = digitalRead(encA_clk_pin);

		if (encA_state != encA_last_state) {
			Serial.print("\r\nNew clk: "); Serial.println(encA_state);
			if (digitalRead(encB_dt_pin) != encA_state) {
				enc_counter++;
				//rotateCW();
			}
			else {
				enc_counter--;
				//rotateCCW();
			}
			Serial.print("\r\nNew enc_counter: "); Serial.println(enc_counter);
		}
		encA_last_state = encA_state;
		check_state();
	}
private:
	uint16_t encA_clk_pin, encB_dt_pin;
	int enc_counter, encA_state, encA_last_state;
	uint16_t step_a1_pin, step_a2_pin, step_b1_pin, step_b2_pin;
	int stepIndex;
	const uint8_t stepArray[8][4] = {
		/* A1, A2, B1, B2 */
		{1,0,0,0},
		{1,0,1,0},
		{0,0,1,0},
		{0,1,1,0},
		{0,1,0,0},
		{0,1,0,1},
		{0,0,0,1},
		{1,0,0,1} };

	// aktualnie wykonywany krok
	int curr_enc_counter = 0;
	// od 4 do 0 - 4 mikrokroki
	int curr_microstep = 0;
	uint64_t last_ms = 0;
	
	void check_state()
	{
		/* 1. OBS£UGA BEZCZYNNOŒCI POMIÊDZY MIKROKROKAMI */
		if (last_ms > 0) {
			// trwa cykl - sprawdzenie czy ju¿ zakoñczyæ
			auto ms = millis();
			if (ms - last_ms >= MICROSTEP_DELAY_ms) {
				// cykl zakoñczony
				last_ms = 0;
			}
			else {
				// czekamy... na nastêpny loop
				return;
			}
		}

		/* 2. OBS£UGA MIKROKROKÓW */
		if (curr_microstep != 0) {
			// trwa cykl - next micro-step
			if (curr_microstep < 0) {
				curr_microstep++;
				set_step(++stepIndex & 0x07);
			}
			else {
				curr_microstep--;
				set_step(--stepIndex & 0x07);
			}
			last_ms = millis();
			return;
		}

		/* 3. OBS£UGA KROKÓW */
		if (enc_counter != curr_enc_counter) {
			// pod¹¿amy za encoderem
			if (enc_counter > curr_enc_counter) {
				curr_enc_counter++;
				curr_microstep = 4;
			}
			else {
				curr_enc_counter--;
				curr_microstep = -4;
			}
		}
	}

	void set_step(int index) {
		Serial.println(index);
		digitalWrite(step_a1_pin, stepArray[index][0]);
		digitalWrite(step_a2_pin, stepArray[index][1]);
		digitalWrite(step_b1_pin, stepArray[index][2]);
		digitalWrite(step_b2_pin, stepArray[index][3]);
		//delayMicroseconds(MICROSTEP_DELAY_US);
	}

	void rotateCW() {
		set_step(++stepIndex & 0x07);
		set_step(++stepIndex & 0x07);
		set_step(++stepIndex & 0x07);
		set_step(++stepIndex & 0x07);
	}

	void rotateCCW() {
		set_step(--stepIndex & 0x07);
		set_step(--stepIndex & 0x07);
		set_step(--stepIndex & 0x07);
		set_step(--stepIndex & 0x07);
	}
};


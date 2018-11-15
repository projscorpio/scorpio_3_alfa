#ifndef HEADER_HPP
#define HEADER_HPP

#define INA_R_0 10
#define PWM_R_0 9
#define INB_R_0 11

#define INA_R_1 22
#define PWM_R_1 27
#define INB_R_1 17

#define INA_L_0 21
#define PWM_L_0 20
#define INB_L_0 16

#define INA_L_1 13
#define PWM_L_1 19
#define INB_L_1 26

#define HIGH 0x1
#define LOW 0x0

std::vector<std::string> split(const std::string s, char delimiter)
{
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter))
        {
                tokens.push_back(token);
        }
        return tokens;
}

class controllerPositions
{
private:
	int axis[4];
	bool  button[4];
	void right_wheel(bool, int);
	void left_wheel(bool, int);
	double right_value_factor(int);
	double left_value_factor(int);

public:
	explicit controllerPositions();
	virtual ~controllerPositions(){};
	void reading(const std::string);
	void setting();
};

controllerPositions::controllerPositions()
{
	for(int i = 0; i < 4; i++) axis[i] = 0;
	for(int i = 0; i < 4; i++) button[i] = false;
}
//Reading from server and decoding function
void controllerPositions::reading(const std::string codedText)
{
	//const std::string codedText="0x8001_0x7FFF_0x8001_0x0";
        std::vector<std::string> results;
        results = split(codedText,'_');
        for(int i=0;i<4;i++)
	{
		std::stringstream ss;
		ss << std::hex << results[i];
		ss >> this->axis[i];
		if(axis[i] > SHRT_MAX) axis[i] = axis[i] - USHRT_MAX;
	}
}
//Reading from class and pinout setting
void controllerPositions::setting()
{
	int pwm_duty_cycle = - ((float)axis[2]/SHRT_MAX*255);
	int turn = - ((float)axis[0]/SHRT_MAX*255);
  double left_factor=left_value_factor(turn), right_factor=right_value_factor(turn);

#ifdef DEBUG
  printf("[Controller Positions][Setting] Pwm:%d: Turn:%d\n", pwm_duty_cycle, turn);
  printf("[Controller Positions][Setting] R_fact:%3f: L_fact:%3f\n", right_factor, left_factor);
#endif

	left_wheel(pwm_duty_cycle*left_factor>0, 
      (int)(left_factor*pwm_duty_cycle));
	right_wheel(pwm_duty_cycle*right_factor>0,
      (int)(right_factor*pwm_duty_cycle));
}

double controllerPositions::right_value_factor(int turn){
	if(turn<0)
		return 1;
	else 
		return (double)(255-2*turn)/255;
}
double controllerPositions::left_value_factor(int turn){
	if(turn>0)
		return 1;
	else 
		return (double)(255+2*turn)/255;
}
void controllerPositions::right_wheel(bool is_forward, int duty_cycle){
	if(is_forward){
		gpioWrite(INA_R_0,LOW);
		gpioWrite(INB_R_0,HIGH);

		gpioWrite(INA_R_1,LOW);
		gpioWrite(INB_R_1,HIGH);
	}else{
		gpioWrite(INA_R_0,HIGH);
		gpioWrite(INB_R_0,LOW);

		gpioWrite(INA_R_1,HIGH);
		gpioWrite(INB_R_1,LOW);
	}
	gpioPWM(PWM_R_0, abs(duty_cycle));
	gpioPWM(PWM_R_1, abs(duty_cycle));
  printf("[Controller Positions][Right Wheel] %s: %d\n", is_forward? "forward": "reverse", duty_cycle);
}

void controllerPositions::left_wheel(bool is_forward, int duty_cycle){
	if(is_forward){
		gpioWrite(INA_L_0,HIGH);
		gpioWrite(INB_L_0,LOW);

		gpioWrite(INA_L_1,HIGH);
		gpioWrite(INB_L_1,LOW);
	}else{
		gpioWrite(INA_L_0,LOW);
		gpioWrite(INB_L_0,HIGH);

		gpioWrite(INA_L_1,LOW);
		gpioWrite(INB_L_1,HIGH);
	}
	gpioPWM(PWM_L_0, abs(duty_cycle));
	gpioPWM(PWM_L_1, abs(duty_cycle));
  printf("[Controller Positions][Left Wheel] %s: %d\n", is_forward? "forward": "reverse", duty_cycle);
}
#endif

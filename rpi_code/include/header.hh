#ifndef HEADER_HPP
#define HEADER_HPP

#define PWM_R 15
#define PWM_L 16
#define INA_R 19
#define INB_R 18
#define INA_L 21
#define INB_L 20
#define HIGH 0x1
#define LOW 0x0

std::string receivingData(void)
{
	std::string ss;
	//Program to receive data from server
	//String returning
	return ss; 
}

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
		if(axis[i] > 32768) axis[i] = axis[i] - 65535;
	}
}
//Reading from class and pinout setting
void controllerPositions::setting()
{
	int pwm_duty_cycle = - ((float)axis[2]/SHRT_MAX*255);
	int turn = ((float)axis[0]/SHRT_MAX*255);
  printf("[Controller Positions][Setting] Pwm:%d: Turn:%d\n", pwm_duty_cycle, turn);


	left_wheel(pwm_duty_cycle*left_value_factor(turn)>0, 
      (int)((double)left_value_factor(turn)*pwm_duty_cycle));
	right_wheel(pwm_duty_cycle*right_value_factor(turn)>0,
      (int)((double)right_value_factor(turn)*pwm_duty_cycle));
}

double controllerPositions::right_value_factor(int turn){
	if(turn<0)
		return 1;
	else 
		return (255-2*turn)/255;
}
double controllerPositions::left_value_factor(int turn){
	if(turn>0)
		return 1;
	else 
		return (-255+2*turn)/255;
}
void controllerPositions::right_wheel(bool is_forward, int duty_cycle){
	if(is_forward){
		gpioWrite(INA_R,LOW);
		gpioWrite(INB_R,HIGH);
	}else{
		gpioWrite(INA_R,HIGH);
		gpioWrite(INB_R,LOW);
	}
	gpioPWM(PWM_R,abs(duty_cycle));
  printf("[Controller Positions][Right Wheel] %s: %d\n", is_forward? "forward": "reverse", duty_cycle);
}

void controllerPositions::left_wheel(bool is_forward, int duty_cycle){
	if(is_forward){
		gpioWrite(INA_L,HIGH);
		gpioWrite(INB_L,LOW);
	}else{
		gpioWrite(INA_L,LOW);
		gpioWrite(INB_L,HIGH);
	}
	gpioPWM(PWM_L,abs(duty_cycle));
  printf("[Controller Positions][Left Wheel] %s: %d\n", is_forward? "forward": "reverse", duty_cycle);
}
#endif

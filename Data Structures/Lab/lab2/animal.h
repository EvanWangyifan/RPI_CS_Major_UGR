#include <string>

class Animal{
public:
	Animal();
 	Animal(std::string aName);

 	//ACCESSOR
 	std::string getName() const;
 	std::string getWeight() const;
 	bool get_survive_land() const;
 	bool get_survive_water() const;
 	bool get_eat_meat() const;
 	bool get_eat_plant() const;

 	//MODIFIER
 	float getWeight(float aWeight);
 	bool set_survive_land(bool on_land);
 	bool set_survive_water(bool in_water);
 	bool set_eat_meat(bool meat);
 	bool set_eat_plant(bool plant);

 	//Other functions
 	//This function returns true only if the animal eat both meat and plant
 	bool is_omnivore() const;{
 		get_eat_plant && get_eat_meat;
 	}
 	//This function returns true only if the animal can both survive in land and water
 	bool is_amphibious() const;{
 		get_survive_water && get_survive_land;
 	}


 private:
 	std::string name;
 	float aWeight;
 	bool on_land;
 	bool in_water;
 	bool meat;
 	bool plant;

}
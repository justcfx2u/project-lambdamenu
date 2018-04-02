const static int CUSTOM_TYRE_COUNT = 1;

const static int WHEEL_CATEGORY_COUNT = 8;

const static std::string WHEEL_CATEGORY_NAMES[] = { "Sports", "Muscle", "Lowrider", "SUV", "Offroad", "Tuner", "High End", "Benny's Originals" };

const static int WHEEL_CATEGORY_COUNTS[] = { 50, 36, 30, 38, 20, 48, 40, 217 }; //{ 25, 18, 15, 19, 10, 24, 20, 51 };

const static int WHEEL_CATEGORY_COUNT_BIKE = 26; //26

const static std::string TINT_NAMES[] = { "No Tint", "Dark", "Medium", "Light", "Very Light", "Safety Value" };

const static std::string PLATE_NAMES[] = { "Blue on White", "Yellow/Black", "Gold/Blue", "Blue/White SA Caps", "Blue/White SA Exempt", "Blue/White Yankton"};

const static int SPECIAL_ID_START = 90; //90

const static int SPECIAL_ID_FOR_PLATE_TEXT = 91;

const static int SPECIAL_ID_FOR_WHEEL_CATEGORY = 92;

const static int SPECIAL_ID_FOR_WHEEL_SELECTION = 93; //92

const static int SPECIAL_ID_FOR_WINDOW_TINT = 94;

const static int SPECIAL_ID_FOR_LICENSE_PLATES = 95;

const static int SPECIAL_ID_FOR_TOGGLE_VARIATIONS = 96;

const static int SPECIAL_ID_FOR_SMOKE = 97;

const static int SPECIAL_ID_FOR_NEONS = 98;

std::string geSpecialItemTitle(int category, int index)
{
	switch (category)
	{
	case SPECIAL_ID_FOR_LICENSE_PLATES:
		return PLATE_NAMES[index];

	case SPECIAL_ID_FOR_WHEEL_CATEGORY:
		return WHEEL_CATEGORY_NAMES[index];

	case SPECIAL_ID_FOR_WINDOW_TINT:
		return TINT_NAMES[index];
	}
	return std::string();
}

bool onconfirm_vehmod_category_menu(MenuItem<int> choice)
{
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if (!bPlayerExists)
	{
		return false;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		set_status_text("Player isn't in a vehicle");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	switch (lastSelectedModValue)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16: //armor
	case 21: 
	case 22: 
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 48:

	{
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);

		std::string modItemNameStr = getNormalItemTitle(veh, lastSelectedModValue, choice.value);

		VEHICLE::SET_VEHICLE_MOD(veh, lastSelectedModValue, choice.value, 1);
		std::ostringstream ss;
		ss << modItemNameStr << " Applied";
		set_status_text(ss.str());
	}
	break;

	case SPECIAL_ID_FOR_WINDOW_TINT: //Change Window Tint
	{
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, choice.value); //Start from beginning
		set_status_text("Changed Window Tint");
	}
	break;

	case SPECIAL_ID_FOR_LICENSE_PLATES: //Change license plate style
	{
		int plateCount = VEHICLE::GET_NUMBER_OF_VEHICLE_NUMBER_PLATES();
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, choice.value); //Start from beginning
		set_status_text("Changed License Plate");
	}
	break;

	case SPECIAL_ID_FOR_WHEEL_CATEGORY: //Change Wheel Category
	{
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, choice.value); //Increment ModValue
		VEHICLE::SET_VEHICLE_MOD(veh, 23, 0, 0);
		set_status_text("Changed Wheel Category");
	}
	break;

	case SPECIAL_ID_FOR_WHEEL_SELECTION: //Change Wheels 
	{
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 23, choice.value, 0); //Remove mod and start from beginning
		VEHICLE::SET_VEHICLE_MOD(veh, 24, choice.value, 0); //Remove mod and start from beginning (For bike rear wheels if they exist)
		set_status_text("Changed Wheels");
	}
	break;
	}
	return false;
}

bool process_vehmod_category_special_menu(int category)
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	std::vector<int> values;

	switch (category)
	{
	case SPECIAL_ID_FOR_LICENSE_PLATES:
	{
		int plateCount = VEHICLE::GET_NUMBER_OF_VEHICLE_NUMBER_PLATES();
		for (int i = 0; i < plateCount; i++)
		{
			values.push_back(i);
		}
	}
		break;
	case SPECIAL_ID_FOR_WHEEL_CATEGORY: // this skips bike wheels category 6 if in a car
	{
		for (int i = 0; i < WHEEL_CATEGORY_COUNT; i++)
		{
			int j = i;
			if (j >= 6)
			{
				j = j + 1; //skip 6
			}
			values.push_back(j);
		}
	}
		break;
	case SPECIAL_ID_FOR_WINDOW_TINT:
	{
		int tintCount = VEHICLE::GET_NUM_VEHICLE_WINDOW_TINTS();
		for (int i = 0; i < tintCount; i++)
		{
			values.push_back(i);
		}
	}
		break;
	default:
		return false;
	}

	MenuItemVector<int> menuItems;
	for (int i = 0; i < values.size(); i++)
	{
		MenuItem<int> item;
		std::string specialName = geSpecialItemTitle(category, i);
		if (!specialName.empty())
		{
			item.caption = specialName;
		}
		else if (i == 0 && values.at(i) == -1)
		{
			item.caption = "Default";
		}
		else
		{
			std::ostringstream ss;
			ss << getModCategoryName(category) << " Item " << i;
			item.caption = ss.str();
		}
		item.value = values.at(i);
		item.isLeaf = true;
		menuItems.push_back(item);
	}

	//Find menu index to return to
	int modChoiceMenuIndex = find_menu_index_to_restore(category, category, veh);

	std::ostringstream ss;
	ss << getModCategoryName(category);

	draw_generic_menu<int>(menuItems, &modChoiceMenuIndex, ss.str(), onconfirm_vehmod_category_menu, NULL, NULL, vehicle_menu_interrupt);

	return false;
}

bool process_vehmod_category_menu(int category)
{
	int actualCategory = category;

	if (category == SPECIAL_ID_FOR_WHEEL_SELECTION)
	{
		actualCategory = 23;
	}
	else if (category > SPECIAL_ID_START)
	{
		return process_vehmod_category_special_menu(category);
	}

	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if (!bPlayerExists)
	{
		return false;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		set_status_text("Player isn't in a vehicle");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	MenuItemVector<int> menuItems;

	int count = 0;
	if (category == SPECIAL_ID_FOR_WHEEL_SELECTION)
	{
		int wheelType = VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh);
		if (wheelType == 6)
		{
			count = WHEEL_CATEGORY_COUNT_BIKE;
		}
		else if (wheelType == 7)
		{
			count = WHEEL_CATEGORY_COUNTS[6]; 
		}
		else if ( wheelType == 8) //8
		{
			count = WHEEL_CATEGORY_COUNTS[7]; //7
		}
		else
		{
			count = WHEEL_CATEGORY_COUNTS[wheelType];
		}
	}
	else
	{
		count = VEHICLE::GET_NUM_VEHICLE_MODS(veh, actualCategory);
	}

	if (category == SPECIAL_ID_FOR_WHEEL_SELECTION)
	{
		MenuItem<int> item;
		item.caption = "Default Wheel For Vehicle";
		item.value = -1;
		item.isLeaf = true;
		menuItems.push_back(item);
	}
	else
	{
		MenuItem<int> item;
		item.caption = getNormalItemTitle(veh, category, -1);
		item.value = -1;
		item.isLeaf = true;
		menuItems.push_back(item);
	}

	for (int i = 0; i < count; i++)
	{
		std::string modItemNameStr = getNormalItemTitle(veh, actualCategory, i);
		MenuItem<int> item;
		item.caption = modItemNameStr;
		item.value = i;
		item.isLeaf = true;
		menuItems.push_back(item);
	}

	//Find menu index to return to
	int modChoiceMenuIndex = find_menu_index_to_restore(category, actualCategory, veh);

	std::ostringstream ss;
	ss << getModCategoryName(lastSelectedModValue);

	draw_generic_menu<int>(menuItems, &modChoiceMenuIndex, ss.str(), onconfirm_vehmod_category_menu, NULL, NULL, vehicle_menu_interrupt);
	return false;
}

bool onconfirm_vehmod_menu(MenuItem<int> choice)
{
	lastSelectedModValue = choice.value;

	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if (!bPlayerExists)
	{
		return false;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		set_status_text("Player isn't in a vehicle");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	switch (choice.value)
	{

bool process_vehmod_menu()
{
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
	{
		set_status_text("Player isn't in a vehicle");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);

	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	Hash vmodel = ENTITY::GET_ENTITY_MODEL(veh);

	BOOL isACar = VEHICLE::IS_THIS_MODEL_A_BIKE(et) || VEHICLE::IS_THIS_MODEL_A_HELI(et) || VEHICLE::IS_THIS_MODEL_A_PLANE(et) || VEHICLE::IS_THIS_MODEL_A_TRAIN(et) || VEHICLE::IS_THIS_MODEL_A_BICYCLE(et) || VEHICLE::IS_THIS_MODEL_A_BOAT(et);
	BOOL isABike = VEHICLE::IS_THIS_MODEL_A_BIKE(et);
	BOOL isAircraft = VEHICLE::IS_THIS_MODEL_A_HELI(et) || VEHICLE::IS_THIS_MODEL_A_PLANE(et);
	BOOL isWeird = VEHICLE::IS_THIS_MODEL_A_TRAIN(et) || VEHICLE::IS_THIS_MODEL_A_BICYCLE(et) || VEHICLE::IS_THIS_MODEL_A_BOAT(et);
	//BOOL isSpecial = (((((((((((((((vmodel == GAMEPLAY::GET_HASH_KEY("PROTOTIPO")) || (vmodel == GAMEPLAY::GET_HASH_KEY("BUCCANEER2"))) || (vmodel == GAMEPLAY::GET_HASH_KEY("PRIMO2"))) || (vmodel == GAMEPLAY::GET_HASH_KEY("MOONBEAM2"))) || (vmodel == GAMEPLAY::GET_HASH_KEY("SABREGT2"))) || (vmodel == GAMEPLAY::GET_HASH_KEY("TORNADO5"))) || (vmodel == GAMEPLAY::GET_HASH_KEY("VOODOO"))) || (vmodel == GAMEPLAY::GET_HASH_KEY("VIRGO2"))) || (vmodel == GAMEPLAY::GET_HASH_KEY("CHINO2"))) || (vmodel == GAMEPLAY::GET_HASH_KEY("MINIVAN2"))) || (vmodel == GAMEPLAY::GET_HASH_KEY("SLAMVAN3"))) || (vmodel == GAMEPLAY::GET_HASH_KEY("FACTION2"))) || (vmodel == GAMEPLAY::GET_HASH_KEY("FACTION3"))) || (vmodel == GAMEPLAY::GET_HASH_KEY("BANSHEE2"))) || (vmodel == GAMEPLAY::GET_HASH_KEY("SULTANRS")));

	std::string caption = "Vehicle Mod Options";

	MenuItemVector<int> menuItems;

	if (!isACar)
	//if (isSpecial)
	{
		MenuItem<int> item7;
		item7.caption = "Accent Color (74)";
		item7.value = -6;
		item7.isLeaf = false;
		menuItems.push_back(item7);

		MenuItem<int> item8;
		item8.caption = "Interior Trim (74)";
		item8.value = -7;
		item8.isLeaf = false;
		menuItems.push_back(item8);
	}

	if (!isWeird && !isAircraft)
	{
		for (int i = 0; i < 49; i++) //63
		{
			if (i == 23 || i == 24 || i == 21) // || i == 50 || i == 51 || i == 52 || i == 53 || i == 60 || i == 55 || i == 54)
			{
				continue;
			}

			bool iFound = false;
			int compIndex = i;

			int mods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, i);
			if (mods > 0)
			{
				std::ostringstream ss;
				//ss << "Slot " << (compIndex + 1) << ": ";
				ss << getModCategoryName(i) << " (" << (mods + 1) << ")";

				MenuItem<int> item;
				item.caption = ss.str();
				item.value = compIndex;
				item.isLeaf = false;
				menuItems.push_back(item);
			}
		}
	}

	std::ostringstream ss;

	if (!isWeird && !isAircraft)
	{
		int tintCount = VEHICLE::GET_NUM_VEHICLE_WINDOW_TINTS();
		MenuItem<int> item;
		ss << getModCategoryName(SPECIAL_ID_FOR_WINDOW_TINT) << " (" << tintCount << ")";
		item.caption = ss.str();
		item.value = SPECIAL_ID_FOR_WINDOW_TINT;
		item.isLeaf = false;
		menuItems.push_back(item);

		ss.str(""); ss.clear();

		int plateCount = VEHICLE::GET_NUMBER_OF_VEHICLE_NUMBER_PLATES();
		ss << getModCategoryName(SPECIAL_ID_FOR_LICENSE_PLATES) << " (" << plateCount << ")";
		item.caption = ss.str();
		item.value = SPECIAL_ID_FOR_LICENSE_PLATES;
		item.isLeaf = false;
		menuItems.push_back(item);

		ss.str(""); ss.clear();

		if (!isWeird && !isAircraft)
		{
			MenuItem<int> item;
			item.caption = "Change Plate Text";
			item.isLeaf = false;
			item.onConfirmFunction = set_plate_text;
			item.value = SPECIAL_ID_FOR_PLATE_TEXT;
			menuItems.push_back(item);
		}

		if (!VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(veh)))
		{
			ss << getModCategoryName(SPECIAL_ID_FOR_WHEEL_CATEGORY) << " (" << WHEEL_CATEGORY_COUNT << ")";
			item.caption = ss.str();
			item.value = SPECIAL_ID_FOR_WHEEL_CATEGORY;
			item.isLeaf = false;
			menuItems.push_back(item);

			ss.str(""); ss.clear();
		}

		int wheelCount = 0;
		int wheelType = VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh);
		if (wheelType == 6)
		{
			wheelCount = WHEEL_CATEGORY_COUNT_BIKE;
		}
		else if (wheelType == 7)
		{
			wheelCount = WHEEL_CATEGORY_COUNTS[6]; //7
		}
		else if (wheelType == 8) 
		{
			wheelCount = WHEEL_CATEGORY_COUNTS[7]; //7
		}
		else
		{
			wheelCount = WHEEL_CATEGORY_COUNTS[wheelType];
		}

		ss << getModCategoryName(SPECIAL_ID_FOR_WHEEL_SELECTION) << " (" << wheelCount << ")";
		item.caption = ss.str();
		item.value = SPECIAL_ID_FOR_WHEEL_SELECTION;
		item.isLeaf = false;
		menuItems.push_back(item);

		ss.str(""); ss.clear();
	}

	if (!isACar)
	{
		MenuItem<int> item5;
		item5.caption = "Tire Smoke (69)";
		item5.value = SPECIAL_ID_FOR_SMOKE;
		item5.isLeaf = false;
		menuItems.push_back(item5);

		MenuItem<int> item6;
		item6.caption = "Neon Lights (39)";
		item6.value = SPECIAL_ID_FOR_NEONS;
		item6.isLeaf = false;
		menuItems.push_back(item6);
	}



	FunctionDrivenToggleMenuItem<int> toggleItem;

	if (!isWeird && !isAircraft)
	{
		toggleItem.caption = "Toggle Turbo Tuning";
		toggleItem.getter_call = is_turbocharged;
		toggleItem.setter_call = set_turbocharged;
		toggleItem.value = SPECIAL_ID_FOR_TOGGLE_VARIATIONS;
		menuItems.push_back(toggleItem);

		toggleItem.caption = "Toggle Xenon Lights";
		toggleItem.getter_call = is_xenon_headlights;
		toggleItem.setter_call = set_xenon_headlights;
		toggleItem.value = SPECIAL_ID_FOR_TOGGLE_VARIATIONS;
		menuItems.push_back(toggleItem);
	}

	if (!isWeird && !isAircraft) // && !isABike
	{
		toggleItem.caption = "Toggle Custom Tires";
		toggleItem.getter_call = is_custom_tyres;
		toggleItem.setter_call = set_custom_tyres;
		toggleItem.value = SPECIAL_ID_FOR_TOGGLE_VARIATIONS;
		menuItems.push_back(toggleItem);
	}

	if (menuItems.size() == 0)
	{
		set_status_text("No relevant mods for this vehicle");
		return false;
	}

	draw_generic_menu<int>(menuItems, 0, "Vehicle Options", onconfirm_vehmod_menu, NULL, NULL, vehicle_menu_interrupt);
	return false;
}

bool is_custom_tyres(std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int tyreCount = VEHICLE::GET_VEHICLE_MOD_VARIATION(veh, 23);
	return (tyreCount != 0);
}

void set_custom_tyres(bool applied, std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
	int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 23);
	VEHICLE::SET_VEHICLE_MOD(veh, 23, currmod, applied); //Add Custom Tires
	VEHICLE::SET_VEHICLE_MOD(veh, 24, currmod, applied); //Add Custom Tires (For bike rear wheels if they exist)
	set_status_text("Changed Tires");
}

bool is_turbocharged(std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	return VEHICLE::IS_TOGGLE_MOD_ON(veh, 18) ? true : false;
}

void set_turbocharged(bool applied, std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	VEHICLE::TOGGLE_VEHICLE_MOD(veh, 18, applied); //Turbo Tuning
}

bool is_xenon_headlights(std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	return VEHICLE::IS_TOGGLE_MOD_ON(veh, 22) ? true : false;
}

void set_xenon_headlights(bool applied, std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, applied); //Headlights
}


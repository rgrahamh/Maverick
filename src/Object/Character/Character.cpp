Character(Stats* stats, Mastery* mastery, Abilities* abilities, Equipment* equipment = NULL, Inventory* inventory = NULL){
	//Setting the struct pointers; these are the non-optional params
	this->stats = stats;
	this->mastery = mastery;
	this->abilities = abilities;

	if(inventory != NULL){
		this->inventory = inventory;
	}else{
		this->inventory = (InvSlot*)calloc(sizeof(InvSlot), 128);
	}
	this->equipment = equipment;

	this->battle_stats = (BattleStats*)malloc(sizeof(BattleStats));

	this->overdrive=false;
}

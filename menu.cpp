#include "menu.h"

void mainMenu(){
	char menuSelection;
	string file;
	//create lists
	list<Assignment> completed;
	list <Assignment> assigned;
	//open file
	cout << "enter name of file to read assignements from: \n";
	getline(cin, file);
	ifstream fin (file);
	if (fin.good())
		cout << "File " << file << "opened successfully!";
	else
	{
		cout << "File " << file << "could not be opened.";
		return;
	}
	//populate lists from file
	readFile(fin, completed, assigned);
	fin.close();

 while (1){ //infinite loop. exits when user selects 0
 	cout << "\nMAIN MENU\n1. View assignments\t\t2. Add assignments\n"
 	<< "3. Edit assignments\t\t4. Complete assignments\n"
 	<< "5. Check late assignments\t6. Save\n"
 	<< "0. Exit\n";

 	cin >> menuSelection;

 	switch (menuSelection){
 		case '1'://View assignments
 		displaySubmenu(completed, assigned);
 		break;
 		case '2': //Add Assignments
 		addSubmenu(completed, assigned);
 		break;
 		case '3': //Edit assignments
 		editSubmenu(assigned);
 		break;
 		case '4': //Complete Assignments
			completeSubmenu(assigned, completed);
 		break;
 		case '5': //Check late assignments
			checkLate(completed);
 		break;
 		case '6': //Save
 		save(file, assigned, completed);
 		break;
 		case '0': //Exit
 		return;
 		default: //Invalid selection
 		cin.clear();
 		cin.ignore(256, '\n');
 		cout << "Invalid selection!\n";
 		menuSelection = '0';
          break; //Restarts while(1) loop
      }
  }
}
void editSubmenu(list<Assignment>& assigned){
	char menuSelection;
	string tAssignmentStr;
	Date tAssignmentDate;
	list<Assignment>::iterator iter;
	string editString;
	bool breakCondition = false;

  while (breakCondition == false){  
	  
  	cout << "\nEDIT ITEM:"
		<< "\n1. Edit due date\t\t2. Edit description\n"
  	<< "0. Back\n";

  	cin >> menuSelection;

	switch (menuSelection){
	case '1':{
		//Edit due date
		cin.clear();
		cin.ignore(256, '\n');
		printList(assigned);
		cout << "Please enter assigned date of assignment you wish to edit: ";
		getline(cin, tAssignmentStr);
		if (isValidStr(tAssignmentStr)){
			tAssignmentDate = tAssignmentStr; //convert string to date object
		}
		else{
			cout << "Invalid date!\n";
			continue;
		}
		//Detect whether or not assignment with specified date exists in list
		if (!assDateExists(assigned, tAssignmentDate)){
			cout << "No assignment with specified date found in Assigned list.\n";
			continue;
		}
		else{
			for (iter = assigned.begin(); iter != assigned.end(); ++iter){
				if (iter->assigned == tAssignmentDate)
					break; //break when iterator points to correct item
			}
		}
		//cin.ignore(256, '\n');
		cout << "Please enter new due date: ";
		getline(cin, editString);
		if (isValidStr(editString))
		{
			tAssignmentDate = editString;
			if (!(tAssignmentDate > iter->assigned)){
				cout << "Due date must be after assigned date!";
				break;
			}
			cout << "Due date changed from " << iter->dueDate;
			iter->dueDate = tAssignmentDate;
			cout << " to " << iter->dueDate << "\n";
			return;//exit editSubmenu
		}
		else{
			cout << "Invalid date!\n";
		}
		break;
	}
	case '2':{
		
		printList(assigned);
		cin.ignore(256, '\n');
		cout << "Please enter assigned date of assignment you wish to edit: \n";
		getline(cin, tAssignmentStr);
		if (isValidStr(tAssignmentStr)){
			tAssignmentDate = tAssignmentStr; //convert string to date object
		}
		else{
			cout << "Invalid date! Press Enter to continue...\n";
			continue;
		}
		//Detect whether or not assignment with specified date exists in list
		if (!assDateExists(assigned, tAssignmentDate)){
			cout << "No assignment with specified date found in Assigned list.\n";
			continue;
		}
		else{
			for (iter = assigned.begin(); iter != assigned.end(); ++iter){
				if (iter->assigned == tAssignmentDate)
					break; //break when iterator points to correct item
			}
		}
		//cin.ignore(256, '\n');
		cout << "Please enter new description (<25 char):\n ";
		getline(cin, editString);
		if (editString.length() < 25){
			cout << "Description changed from \"" << iter->description
				<< "\" to \"";
			iter->description = editString;
			cout << iter->description << "\"!\n";
		}
		else{
			cout << "Description too long. Please use 24 characters or less.\n";
		}
		break;
	}
  		case '0':
        return;//exit to previous menu
        default:
        cin.clear();
        cin.ignore(256, '\n');
        cout << "Invalid selection!\n";
        menuSelection = '0';
          break; //restarts while(1) loop
      }
  }
}

void displaySubmenu(list<Assignment> & completed, list<Assignment>& assigned){
	char menuSelection;

    while (1){//dangerous retard code
    	cout << "\nDISPLAY MENU:\n1. Display completed\t\t2. Display assigned\n"
    	<< "0. Back\n";

    	cin >> menuSelection;

    	switch (menuSelection){
    		case '1':
    		printList(completed);
    		break;
    		case '2':
    		printList(assigned);
    		break;
    		case '0':
                return;//exit to previous menu
                default:
                cin.clear();
                cin.ignore(256, '\n');
                cout << "Invalid selection!\n";
                menuSelection = '0';
                break; //restarts while(1) loop
            }
        }
    }

void addSubmenu(list<Assignment>& completed, list<Assignment>& assigned){
    char menuSelection;

 while (1){//dangerous retard code
    cout << "\nADD ASSIGNMENT:\n1. Add completed\t\t2. Add assigned\n"
    << "0. Back\n";

    cin >> menuSelection;

    switch (menuSelection){
    	case '1':
			addAssignment(completed, assigned, 1);
    	break;
		case '2':{
			addAssignment(assigned, completed, 0);
		}
    	break;
    	case '0':
            return;//exit to previous menu
            default:
            cin.clear();
            cin.ignore(256, '\n');
            cout << "Invalid selection!\n";
            menuSelection = '0';
            break; //restarts while(1) loop
        }
    }
}
void readFile(ifstream& fin, list <Assignment>& completed, list <Assignment>& assigned){
//Get line, parse info, add to appropriate list
    string dueDate, description, assignedDate, status;
	string rawString;
//Maybe insert a thing that clears out the lists just in case there's stuff in them for some reason.
    while (!fin.eof())
    {
		getline(fin, rawString);
		parseInput(dueDate, description, assignedDate, status, rawString);

	Date newDue = dueDate;
	Date newAss = assignedDate;
	if (status == "assigned")//decide which list to add to.
	{
		Assignment newAssignment(newAss, newDue, description, Assignment::Status::ASSIGNED);
		assigned.push_back(newAssignment);
	}
	else if (status == "completed")
	{
		Assignment newAssignment(newAss, newDue, description, Assignment::Status::COMPLETED);
		completed.push_back(newAssignment);
	}
	else if (status == "late")
	{
		Assignment newAssignment(newAss, newDue, description, Assignment::Status::LATE);
		completed.push_back(newAssignment);
	}
	else //Print error to console if weird status value is found.
	{
		cout << "File read unsuccessful!\nCould not parse item with status " << status << "!\n";
		return;
	}
}
return;
}

void printList(list<Assignment> assList)//teehee
{
	//check if the list is empty
	if (assList.empty())
	{
		cout << "No assignments found.\n";
		return;
	}
	else
	{	//print collumn headers
		cout << "ASSIGNED   DESCRIPTION             DUE       STATUS\n";
		//iterate through list printing and stuff.
		for (list<Assignment>::iterator iter = assList.begin(); iter != assList.end(); ++iter)
		{
			//Print assigned date and description
			cout << ' ' << setw(8) << setfill('0') << right << iter->assigned << "   "
				<< setfill(' ') << left
				<< setw(24) << iter->description
				<< setw(8) << setfill('0') << right << iter->dueDate << "   ";
			//Print status
			if (iter->status == 0)
				cout << "ASSIGNED";
			else if (iter->status == 1)
				cout << "COMPLETED";
			else if (iter->status == 2)
				cout << "LATE";
			cout << '\n';

		}
	}
}
void parseInput(string& dueDate, string& description, string& assignedDate, string& status, string line)
{
	string tempDate;
	//get the date from the line
	tempDate = line.substr(0, line.find_first_of(','));
	dueDate = parseDate(tempDate);
	//delete first date and ", " from string
	line.erase(0, line.find_first_of(',') + 2);
	//get the description
	description = line.substr(0, line.find_first_of(','));
	
	line.erase(0, line.find_first_of(',') + 2);
	//get the other date
	tempDate = line.substr(0, line.find_first_of(','));
	assignedDate = parseDate(tempDate);
	line.erase(0, line.find_first_of(',') + 2);
	//get the status
	status = line;
	
	return;
	
}

string parseDate(string date)
{
	//create temp variables
	string year, month, day, returnDate;
	month = date.substr(0, date.find_first_of('-'));
	date.erase(0, date.find_first_of('-')+1);
	day = date.substr(0, date.find_first_of('-'));
	date.erase(0, date.find_first_of('-')+1);
	year = date;
	
	if (month.length() < 2)
		month = '0' + month;
	if (day.length() < 2)
		day = '0' + day;
	returnDate = year + month + day;

	return returnDate; //returns format YYYYMMDD
}
//assAssignment is dual function. Behaves differently depending on bool parameter.
void addAssignment(list<Assignment>& theList, list<Assignment>& theOtherlist, bool isCompleted){
	Date assigned, due;
	string date, description, status;
	char statChar;
	bool inserted = false;
	cin.ignore(256, '\n');
	//menu header
	if (isCompleted == 1)
		cout << "ADD COMPLETED ASSIGNMENT:\n";
	else
		cout << "ADD NEW ASSIGNMENT:\n";
	while (1){//loop hassles user until they finally enter a valid date
		cout << "Please enter assigned date(YYYYMMDD): ";
		getline(cin, date);//get input
		//test 
		if (isValidStr(date)){
			assigned = date;//we have our assigned date
			cout << '\n';
			break;
		}
		else if (assDateExists(theList, date)){
			cout << "An assignment with that date already exists in the ";
			if (isCompleted){
				cout << "Completed list!\n";
			}
			else
				cout << "Assigned list!\n";
			continue;
		}
		else if (assDateExists(theOtherlist, date)){
			cout << "An assignment with that date already exists in the ";
			if (isCompleted){
				cout << "Assigned list!\n";
			}
			else{
				cout << "Completed list!\n";
			}
		}
		else{
			cout << "Invalid date. Please try again.\n ";
			continue;
		}	
	}
	while (1){ //hassles user for description
		cout << "Please enter description (<=24 characters): ";
		getline(cin, description);
		if (description.length() < 25){
			cout << '\n';
			break;
		}
		else{
			cout << "Too many characters.\n ";
			continue;
		}
	}

	while (1){//hassles user for due date
		cout << "Please enter due date(YYYYMMDD): ";
		getline(cin, date);
		if (isValidStr(date) && (stoi(date) > assigned.toInt())){
			due = date;
			cout << '\n';
			break;
		}
		else{
			cout << "Invalid date. Please try agian.\n";
			continue;
		}
	}
	Assignment newAssignment;
	while (1){//hassles user for status unless we're adding an ASSIGNED object
		if (isCompleted == 1)
		cout << "Please enter status\nC = Completed\nL = Late\nStatus: ";
		else{//if we're adding an ASSIGNED type, we know what the status will be
			newAssignment.assigned = assigned;
			newAssignment.dueDate = due;
			newAssignment.description = description;
			newAssignment.status = Assignment::Status::ASSIGNED;
			break;
		}
		cin >> statChar;
		if (statChar == 'C' || statChar == 'c'){
			newAssignment.assigned = assigned;
			newAssignment.dueDate = due;
			newAssignment.description = description;
			newAssignment.status = Assignment::Status::COMPLETED;
			break;
		}
		else if (statChar == 'L' || statChar == 'l'){
			newAssignment.assigned = assigned;
			newAssignment.dueDate = due;
			newAssignment.description = description;
			newAssignment.status = Assignment::Status::LATE;
			break;
		}
		else{//incorrect input
			cin.clear();
			cin.ignore(256, '\n');
			cout << "Invalid selection!\n";
			continue;
		}		
	}
	//insert new object into list
	if (theList.empty() /*|| newAssignment.dueDate > (theList.end()).dueDate*/){

		theList.push_back(newAssignment);
		inserted = true;
		cout << "Assignment added to list!\n";
	}
	else
		for (list<Assignment>::iterator iter = theList.begin(); iter != theList.end();){
		if (newAssignment.dueDate < iter->dueDate){
			cout << "Assignment added to list!\n";
			theList.insert(iter, newAssignment);
			inserted = true;
			break;
			}
		++iter;
		}
	if (inserted == false)
	{
		theList.push_back(newAssignment);
		cout << "Assignment added to list!\n";
	}
	return;
}
bool isValidStr(const string str){//tests validity of string passed as date.
	int mo, dy;
	if (str.length() != 8){//check for proper length
		return false;
	}
	for (int i = 0; i < 8; ++i){//check if entire string is numeric
		//assumes correct length since it got this far
		if (!isdigit(str[i])){
			return false;
		}
	}
	mo = stoi(str.substr(4, 2));//get month as int
	///*DEBUG*/cout << "mo: " << mo << '\n';
	if (mo > 12 || mo < 1){//check for valid month
		return false;
	}
	dy = stoi(str.substr(6, 2));//get day as int
	///*DEBUG*/ cout << "dy: " << dy << '\n';
	if (dy < 1){
		return false;
	}
	//checks to see if dates have the appropriate number of days based on month
	switch (mo){
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		if (dy > 31){
			return false;
		}
		else
			break;
	case 4:
	case 6:
	case 9:
	case 11:
		if (dy > 30){
			return false;
		}
		else
			break;
	case 2:
		if (dy > 28){
			return false;
		}
		else
			break;
	}
	return true;
}
bool assDateExists(list<Assignment> theList, Date date){
	for (list<Assignment>::iterator iter = theList.begin(); iter != theList.end(); ++iter){
		if (iter->assigned == date)
			return true;
	}
	return false;
}
void save(string fileName, list<Assignment> assigned, list<Assignment> completed){
	ofstream fout;
	fout.open(fileName);
	//print completed list to file
	for (list<Assignment>::iterator iter = completed.begin(); iter != completed.end(); ++iter)
	{
		fout << iter->assigned.toFileString() << ", "
			<< iter->description << ", "
			<< iter->dueDate.toFileString() << ", ";
		if (iter->status == Assignment::Status::COMPLETED)
			fout << "completed\n";
		else if (iter->status == Assignment::Status::LATE)
			fout << "late\n";
	}
	//print assigned list to file
	for (list<Assignment>::iterator iter = assigned.begin(); iter != assigned.end(); ++iter)
	{
		fout << iter->assigned.toFileString() << ", "
			<< iter->description << ", "
			<< iter->dueDate.toFileString() << ", "
			<< "assigned";
		++iter;
		if (iter != assigned.end())//make sure there's no trailing nonsense
			fout << '\n';
		--iter;
	}
	fout.close();
	cout << "Changes saved to " << fileName << "!\n";
}
void checkLate(list<Assignment> completed){
	int count = 0;
	cout << "LATE ASSIGNMENTS:\nASSIGNED   DESCRIPTION             DUE\n";
	for (list<Assignment>::iterator iter = completed.begin(); iter != completed.end(); ++iter){
		if (iter->status == Assignment::Status::LATE){
			cout << ' ' << left << setw(11) << iter->assigned
				<< left << setw(24) << iter->description
				<< left << setw(10) << iter->dueDate << '\n';
			++count;
		}
	}
	cout << count << " late assignments found.\n";
}
void completeSubmenu(list<Assignment>& assigned, list<Assignment>& completed){
	bool breakCondition = false, inserted = false;
	string completeString, targetString;
	Date completeDate, targetDate;
	list<Assignment>::iterator assIter, compIter;
	Assignment tempAss;

	cin.ignore(256, '\n');
	cin.clear();

	while (breakCondition == false){
		printList(assigned);
		cout << "Please enter assigned date of assignment to be completed\n"
			<< "or type \"0\" to exit to previous menu.\n";
		getline(cin, targetString);
		//exit condition
		if (targetString == "0"){
			breakCondition = true;
			continue;
		}
		//ensure user input is a valid date
		if (isValidStr(targetString)){
			targetDate = targetString;
		}
		else{
			cout << "Invalid date!\n";
			continue;
		}
		//ensure item exists in assigned list
		if (!assDateExists(assigned, targetDate)){
			cout << "No assignment with specified date found in Assigned list.\n";
			continue;
		}
		for (assIter = assigned.begin(); assIter != assigned.end(); ++assIter)
		{
			if (assIter->assigned == targetDate){
				break;//break once target object is found
			}
		}
		cout << "Completing assignment \"" << assIter->description << "\".\n"
			<< "Please enter completion date: ";
		getline(cin, completeString);
		//ensure user input is valid date
		if (isValidStr(completeString)){
			completeDate = completeString;
		}
		else{
			cout << "Invalid date!";
			continue;
		}
		//ensure complete date is greater than assigned date
		if (!(completeDate > assIter->assigned)){
			cout << "Complete date must be greater than assigned date!\n";
			continue;
		}
		//if all criteria satisfied, insert assignment into completed list
		//but first, copy the assignment object manually for some reason
		tempAss.assigned = assIter->assigned;
		tempAss.description = assIter->description;
		tempAss.dueDate = assIter->dueDate;
		if (completeDate > assIter->dueDate)//determine if late
			tempAss.status = Assignment::Status::LATE;
		else{
			tempAss.status = Assignment::Status::COMPLETED;
		}
		for (compIter = completed.begin(); compIter != completed.end(); ++compIter){
			if (compIter->assigned > tempAss.assigned){
				completed.insert(compIter, tempAss);
				inserted = true;
				cout << "Assignment \"" << tempAss.description << "\" completed ";
				if (tempAss.status == Assignment::Status::LATE)
					cout << " LATE!\n";
				else
					cout << " ON TIME!\n";
				break;
			}
		}
		//if assignment is to be the last item in the list
		if (inserted == false){
			completed.push_back(tempAss);
			inserted = true;
			cout << "Assignment \"" << tempAss.description << "\" completed ";
			if (tempAss.status == Assignment::Status::LATE)
				cout << " LATE!\n";
			else
				cout << " ON TIME!\n";
		}
		//This is also where we'll erase the assignment from the assigned list
		assigned.erase(assIter);
		break;
	}
	
}

File Struktur UniSax 


// --------- General -------------------

BasicSaxParser.h		
BasicReader.h

BasicFormatter.h
BasicWriter.h

// --------- XML -----------------------

XmlInfo.h

// Read
XmlParser.h/.cpp
XmlReader.h/.cpp

//Write
XmlWriter.h/.cpp
XmlFormatter.h/.cpp


// ---------- JSON ---------------------

JsonInfo.h

// Read
JsonParser.h/.cpp
JsonReader.h/.cpp

// Write
JsonWriter.h/.cpp
JsonFormatter.h/.cpp

// ---------------- weitere --------------


Aktueller stand Json:

| Datei              | Zustand 
|------------------- | -------------------
| BasicSaxParser.h   | OK 
| JsonInfo.h         | Ok 
| JsonParser.h       | OK 
| JsonParser.cpp     | OK 
| JsonReader.h   	 | OK 
| JsonReader.cpp     | OK 
| BasicFormatter.h   | OK 
| BasicWriter.h      | OK 
| JsonFormatter.h    | OK 
| JsonFormatter.cpp  | OK 
| JsonWriter.h       | OK 
| JsonWriter.cpp     | OK 



Aktueller stand auf : https://github.com/hgl9910/UniSax 

Aktueller stand  XML : noch nicht im Repository

Lebensdauer von objekten 


************************  fehlt noch **************************************************

JsonReader
Read(const std::string& filename);
Read(std::istream& input);

File öffnen implemetiert in BasicReader 

offen file close und fehlerbehandlung 

!! 
Eine wichtige Feinheit: Bei einem vom Benutzer übergebenen Stream darf BasicReader ihn nicht schließen. 
Bei einem intern geöffneten ifstream wird dessen Lebensdauer dagegen von Read() kontrolliert.

Das gleiche gilt spiegelbildlich für BasicWriter und ostream.

JsonReader
   │
   ├── Read(std::istream&)
   │
   └── Read(filename)
          │
          ▼
     std::ifstream
          │
          ▼
      JsonParser
	  	  
	  
	  
JsonWriter
   │
   ├── Write(std::ostream&)
   │
   └── Write(filename)
          │
          ▼
     std::ofstream
          │
          ▼
     JsonFormatter	  
	 
	 
	 
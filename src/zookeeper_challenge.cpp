// Zookeeper's Challenge: this program reads new animal arrivals, organizes them by
// habitat, and writes a detailed population report using basic C++ OOP features.

#include <algorithm>  // gives access to helpful functions like std::find_if_not
#include <cctype>     // lets us use std::isspace and std::tolower
#include <fstream>    // used to read and write text files
#include <iomanip>    // helps format numbers such as dates
#include <iostream>   // enables basic input/output with std::cout
#include <map>        // stores key/value pairs, perfect for species counters
#include <memory>     // manages dynamically created objects safely
#include <sstream>    // allows string-based streams for parsing text
#include <stdexcept>  // defines standard exception classes
#include <string>     // represents and manipulates text
#include <utility>    // provides std::move for efficient transfers
#include <vector>     // dynamic array used for holding animals and names

// ---------------------------------------------------------------------------
// Helper functions live inside an unnamed namespace so they remain private to
// this file. These tiny tools keep the main logic easier to follow.
// ---------------------------------------------------------------------------

namespace {

// trim removes spaces from the front and back of a string.
std::string trim(const std::string &input) {
    const auto first = std::find_if_not(input.begin(), input.end(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    });
    const auto last = std::find_if_not(input.rbegin(), input.rend(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    }).base();
    if (first >= last) {
        return "";
    }
    return std::string(first, last);
}

// toLower makes a new string where every letter is lowercase.
std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

// split chops a string into parts whenever it finds the delimiter text.
std::vector<std::string> split(const std::string &value, const std::string &delimiter) {
    std::vector<std::string> tokens;
    std::string remaining = value;
    std::size_t pos = 0;
    while ((pos = remaining.find(delimiter)) != std::string::npos) {
        tokens.emplace_back(remaining.substr(0, pos));
        remaining.erase(0, pos + delimiter.size());
    }
    if (!remaining.empty()) {
        tokens.emplace_back(remaining);
    }
    return tokens;
}

// Date stores a simple calendar date broken into year/month/day numbers.
struct Date {
    int year{};
    int month{};
    int day{};
};

// parseIsoDate reads a date written like 2024-04-02 and splits it into pieces.
Date parseIsoDate(const std::string &value) {
    std::istringstream ss(value);
    Date result{};
    char dash1 = 0;
    char dash2 = 0;
    if (!(ss >> result.year >> dash1 >> result.month >> dash2 >> result.day) || dash1 != '-' || dash2 != '-') {
        throw std::runtime_error("Invalid ISO date: " + value);
    }
    return result;
}

// formatIsoDate builds a readable string like 2024-03-05 from a Date object.
std::string formatIsoDate(const Date &date) {
    std::ostringstream out;
    out << std::setw(4) << std::setfill('0') << date.year << '-' << std::setw(2) << std::setfill('0')
        << date.month << '-' << std::setw(2) << std::setfill('0') << date.day;
    return out.str();
}

// genBirthDay estimates an animal's birthday using its age, a season, and arrival date.
std::string genBirthDay(int age, const std::string &season, const std::string &arrivalDate) {
    const Date arrival = parseIsoDate(arrivalDate);
    Date birthDate{};
    birthDate.year = arrival.year - age;

    static const std::map<std::string, std::pair<int, int>> seasonToDate = {
        {"spring", {3, 15}}, {"summer", {6, 15}}, {"fall", {9, 15}},
        {"autumn", {9, 15}}, {"winter", {12, 15}}
    };

    const auto normalizedSeason = toLower(season);
    const auto iter = seasonToDate.find(normalizedSeason);
    if (iter != seasonToDate.end()) {
        birthDate.month = iter->second.first;
        birthDate.day = iter->second.second;
    } else {
        // If we do not know the season we reuse the arrival month and day.
        birthDate.month = arrival.month;
        birthDate.day = arrival.day;
    }

    return formatIsoDate(birthDate);
}

// speciesPrefix returns the two-letter prefix used when building unique IDs.
std::string speciesPrefix(const std::string &species) {
    const std::string lowered = toLower(species);
    if (lowered == "hyena") {
        return "Hy";
    }
    if (lowered == "lion") {
        return "Li";
    }
    if (lowered == "tiger") {
        return "Ti";
    }
    if (lowered == "bear") {
        return "Be";
    }
    throw std::runtime_error("Unsupported species: " + species);
}

// genUniqueId keeps track of how many animals belong to each species and
// returns values like Hy01 or Li03.
std::string genUniqueId(const std::string &species, std::map<std::string, int> &counters) {
    const std::string prefix = speciesPrefix(species);
    const int next = ++counters[prefix];
    std::ostringstream out;
    out << prefix << std::setw(2) << std::setfill('0') << next;
    return out.str();
}

} // namespace

// ---------------------------------------------------------------------------
// Animal hierarchy: the base class stores details shared by every animal while
// the subclasses only add very small tweaks like the social group label.
// ---------------------------------------------------------------------------

class Animal {
  public:
    // The constructor fills in every shared detail for an animal.
    Animal(std::string name, std::string species, std::string uniqueId, int age, std::string sex,
           std::string color, int weight, std::string origin, std::string arrivalDate,
           std::string birthDate)
        : name_(std::move(name)), species_(std::move(species)), uniqueId_(std::move(uniqueId)),
          age_(age), sex_(std::move(sex)), color_(std::move(color)), weight_(weight),
          origin_(std::move(origin)), arrivalDate_(std::move(arrivalDate)),
          birthDate_(std::move(birthDate)) {}

    virtual ~Animal() = default;

    // Getter functions expose private data in a safe, read-only way.
    const std::string &name() const { return name_; }
    const std::string &species() const { return species_; }
    const std::string &uniqueId() const { return uniqueId_; }
    int age() const { return age_; }
    const std::string &sex() const { return sex_; }
    const std::string &color() const { return color_; }
    int weight() const { return weight_; }
    const std::string &origin() const { return origin_; }
    const std::string &arrivalDate() const { return arrivalDate_; }
    const std::string &birthDate() const { return birthDate_; }

    // Every derived class must say where the animal lives and its group label.
    virtual std::string habitatName() const = 0;
    virtual std::string socialGroupLabel() const = 0;

    // reportLine formats one line of text for the output file.
    std::string reportLine() const {
        std::ostringstream out;
        out << uniqueId_ << "; " << name_ << "; birth date " << birthDate_ << "; " << color_
            << " color; " << sex_ << "; " << weight_ << " pounds; from " << origin_ << "; arrived "
            << arrivalDate_;
        return out.str();
    }

  protected:
    // setSocialGroup lets subclasses record a clan/pride/etc. string.
    void setSocialGroup(std::string value) { socialGroup_ = std::move(value); }
    const std::string &socialGroup() const { return socialGroup_; }

  private:
    // Private data members hold the actual details.
    std::string name_;
    std::string species_;
    std::string uniqueId_;
    int age_{};
    std::string sex_;
    std::string color_;
    int weight_{};
    std::string origin_;
    std::string arrivalDate_;
    std::string birthDate_;
    std::string socialGroup_;
};

class Hyena : public Animal {
  public:
    // The Hyena constructor forwards most work to the Animal base class.
    Hyena(std::string name, std::string uniqueId, int age, std::string sex, std::string color,
          int weight, std::string origin, std::string arrivalDate, std::string birthDate,
          std::string clan)
        : Animal(std::move(name), "Hyena", std::move(uniqueId), age, std::move(sex),
                 std::move(color), weight, std::move(origin), std::move(arrivalDate),
                 std::move(birthDate)) {
        setSocialGroup(std::move(clan));
    }

    std::string habitatName() const override { return "Hyena Habitat"; }
    std::string socialGroupLabel() const override { return "Clan: " + socialGroup(); }
};

class Lion : public Animal {
  public:
    Lion(std::string name, std::string uniqueId, int age, std::string sex, std::string color,
         int weight, std::string origin, std::string arrivalDate, std::string birthDate,
         std::string pride)
        : Animal(std::move(name), "Lion", std::move(uniqueId), age, std::move(sex),
                 std::move(color), weight, std::move(origin), std::move(arrivalDate),
                 std::move(birthDate)) {
        setSocialGroup(std::move(pride));
    }

    std::string habitatName() const override { return "Lion Habitat"; }
    std::string socialGroupLabel() const override { return "Pride: " + socialGroup(); }
};

class Tiger : public Animal {
  public:
    Tiger(std::string name, std::string uniqueId, int age, std::string sex, std::string color,
          int weight, std::string origin, std::string arrivalDate, std::string birthDate,
          std::string ambush)
        : Animal(std::move(name), "Tiger", std::move(uniqueId), age, std::move(sex),
                 std::move(color), weight, std::move(origin), std::move(arrivalDate),
                 std::move(birthDate)) {
        setSocialGroup(std::move(ambush));
    }

    std::string habitatName() const override { return "Tiger Habitat"; }
    std::string socialGroupLabel() const override { return "Ambush: " + socialGroup(); }
};

class Bear : public Animal {
  public:
    Bear(std::string name, std::string uniqueId, int age, std::string sex, std::string color,
         int weight, std::string origin, std::string arrivalDate, std::string birthDate,
         std::string sleuth)
        : Animal(std::move(name), "Bear", std::move(uniqueId), age, std::move(sex),
                 std::move(color), weight, std::move(origin), std::move(arrivalDate),
                 std::move(birthDate)) {
        setSocialGroup(std::move(sleuth));
    }

    std::string habitatName() const override { return "Bear Habitat"; }
    std::string socialGroupLabel() const override { return "Sleuth: " + socialGroup(); }
};

// ---------------------------------------------------------------------------
// File helpers read lists of names, grab a name for each animal, and assign a
// simple social group string so every species feels unique.
// ---------------------------------------------------------------------------

// loadAnimalNames reads the animalNames.txt file into a map of species -> names.
std::map<std::string, std::vector<std::string>> loadAnimalNames(const std::string &path) {
    std::ifstream input(path);
    if (!input.is_open()) {
        throw std::runtime_error("Unable to open name file: " + path);
    }

    std::map<std::string, std::vector<std::string>> names;
    std::string line;
    while (std::getline(input, line)) {
        line = trim(line);
        if (line.empty()) {
            continue;
        }

        const auto colonPos = line.find(':');
        if (colonPos == std::string::npos) {
            throw std::runtime_error("Expected ':' in name line: " + line);
        }

        const std::string species = toLower(trim(line.substr(0, colonPos)));
        const std::string values = trim(line.substr(colonPos + 1));
        const auto tokens = split(values, ",");
        std::vector<std::string> trimmedTokens;
        trimmedTokens.reserve(tokens.size());
        for (const auto &token : tokens) {
            const std::string cleaned = trim(token);
            if (!cleaned.empty()) {
                trimmedTokens.push_back(cleaned);
            }
        }
        names[species] = std::move(trimmedTokens);
    }

    return names;
}

// popNextName removes and returns the next unused name for a species.
std::string popNextName(std::map<std::string, std::vector<std::string>> &names,
                        const std::string &speciesKey) {
    auto &pool = names[speciesKey];
    if (!pool.empty()) {
        const std::string next = pool.front();
        pool.erase(pool.begin());
        return next;
    }
    return "Unnamed " + speciesKey;
}

// selectSocialGroup rotates through a small list of group names for variety.
std::string selectSocialGroup(const std::string &speciesKey, int index) {
    static const std::map<std::string, std::vector<std::string>> groups = {
        {"hyena", {"Motto Clan", "Serengeti Clan", "Savannah Clan", "Spotted Clan"}},
        {"lion", {"Golden Pride", "Savanna Pride", "Sunset Pride", "River Pride"}},
        {"tiger", {"Ember Ambush", "Jungle Ambush", "River Ambush", "Shadow Ambush"}},
        {"bear", {"Highland Sleuth", "Forest Sleuth", "Mountain Sleuth", "Valley Sleuth"}},
    };

    const auto iter = groups.find(speciesKey);
    if (iter == groups.end() || iter->second.empty()) {
        return "Unknown";
    }
    const auto &options = iter->second;
    return options[index % static_cast<int>(options.size())];
}

// ParsedAnimalRow stores the raw facts we pull from each line of arrivingAnimals.txt.
struct ParsedAnimalRow {
    std::string arrivalDate;
    int age{};
    std::string sex;
    std::string species;
    std::string birthSeason;
    std::string color;
    int weight{};
    std::string origin;
};

// parseArrivalRow breaks an input sentence into individual pieces we can use later.
ParsedAnimalRow parseArrivalRow(const std::string &line) {
    ParsedAnimalRow row;
    const auto parts = split(line, ", ");
    if (parts.size() < 6) {
        throw std::runtime_error("Malformed arrival entry: " + line);
    }

    // Part 0 is the arrival date in ISO format.
    row.arrivalDate = trim(parts[0]);

    // Part 1 includes the age, sex, and species description.
    {
        std::istringstream ss(parts[1]);
        std::string yearToken;
        std::string oldToken;
        ss >> row.age >> yearToken >> oldToken >> row.sex >> row.species;
        row.sex = toLower(row.sex);
        row.species = toLower(row.species);
        if (ss.fail()) {
            throw std::runtime_error("Unable to parse age/sex/species segment: " + parts[1]);
        }
    }

    // Part 2 states the birth season (or says "born in ...").
    {
        const std::string seasonSection = toLower(parts[2]);
        if (seasonSection.find("born in") != std::string::npos) {
            std::istringstream ss(seasonSection);
            std::string bornToken;
            std::string inToken;
            ss >> bornToken >> inToken >> row.birthSeason;
        } else {
            row.birthSeason = trim(parts[2]);
        }
        if (row.birthSeason.empty()) {
            row.birthSeason = "unknown";
        }
    }

    // Part 3 describes the color and usually includes the word "color".
    {
        const std::string colorSection = toLower(parts[3]);
        const auto colorPos = colorSection.find(" color");
        if (colorPos != std::string::npos) {
            row.color = trim(parts[3].substr(0, colorPos));
        } else {
            row.color = trim(parts[3]);
        }
    }

    // Part 4 begins with the weight before the word "pounds".
    {
        std::string weightSection = parts[4];
        const auto spacePos = weightSection.find(' ');
        if (spacePos != std::string::npos) {
            weightSection = weightSection.substr(0, spacePos);
        }
        row.weight = std::stoi(weightSection);
    }

    // Everything after part 4 lists the city, park, and country of origin.
    {
        std::string origin;
        for (std::size_t i = 5; i < parts.size(); ++i) {
            if (!origin.empty()) {
                origin += ", ";
            }
            origin += parts[i];
        }
        const std::string prefix = "from ";
        if (toLower(origin).rfind(prefix, 0) == 0) {
            origin = origin.substr(prefix.size());
        }
        row.origin = trim(origin);
    }

    return row;
}

// ---------------------------------------------------------------------------
// main ties everything together: load files, build animals, and write the report.
// ---------------------------------------------------------------------------

int main() {
    try {
        // Set up the file paths used throughout the program.
        const std::string namesPath = "data/animalNames.txt";
        const std::string arrivalsPath = "data/arrivingAnimals.txt";
        const std::string reportPath = "zooPopulation.txt";

        // Read the pool of possible names and open the arrivals list.
        auto names = loadAnimalNames(namesPath);
        std::ifstream arrivalFile(arrivalsPath);
        if (!arrivalFile.is_open()) {
            throw std::runtime_error("Unable to open arrivals file: " + arrivalsPath);
        }

        // Containers that keep animals, pointers grouped by habitat, and counters.
        std::vector<std::unique_ptr<Animal>> animals;
        std::map<std::string, std::vector<Animal *>> animalsByHabitat;
        std::map<std::string, int> idCounters;
        std::map<std::string, int> speciesCounts;

        // Process each line of the arrivals file one by one.
        std::string line;
        while (std::getline(arrivalFile, line)) {
            line = trim(line);
            if (line.empty()) {
                continue;
            }

            const ParsedAnimalRow row = parseArrivalRow(line);
            const std::string uniqueId = genUniqueId(row.species, idCounters);
            const std::string name = popNextName(names, row.species);
            const std::string birthDate = genBirthDay(row.age, row.birthSeason, row.arrivalDate);
            const std::string group = selectSocialGroup(row.species, speciesCounts[row.species]);

            // Create the correct subtype object based on the species name.
            std::unique_ptr<Animal> created;
            if (row.species == "hyena") {
                created = std::make_unique<Hyena>(name, uniqueId, row.age, row.sex, row.color,
                                                  row.weight, row.origin, row.arrivalDate,
                                                  birthDate, group);
            } else if (row.species == "lion") {
                created = std::make_unique<Lion>(name, uniqueId, row.age, row.sex, row.color,
                                                 row.weight, row.origin, row.arrivalDate,
                                                 birthDate, group);
            } else if (row.species == "tiger") {
                created = std::make_unique<Tiger>(name, uniqueId, row.age, row.sex, row.color,
                                                  row.weight, row.origin, row.arrivalDate,
                                                  birthDate, group);
            } else if (row.species == "bear") {
                created = std::make_unique<Bear>(name, uniqueId, row.age, row.sex, row.color,
                                                 row.weight, row.origin, row.arrivalDate,
                                                 birthDate, group);
            } else {
                throw std::runtime_error("Encountered unsupported species: " + row.species);
            }

            // Update the tracking maps and remember the new object.
            speciesCounts[row.species]++;
            animalsByHabitat[created->habitatName()].push_back(created.get());
            animals.push_back(std::move(created));
        }

        // Prepare to write the final report file.
        std::ofstream report(reportPath);
        if (!report.is_open()) {
            throw std::runtime_error("Unable to open report for writing: " + reportPath);
        }

        // Print each habitat in a fixed order so the report is easy to read.
        const std::vector<std::string> habitatOrder = {"Hyena Habitat", "Lion Habitat",
                                                       "Tiger Habitat", "Bear Habitat"};
        for (const auto &habitat : habitatOrder) {
            report << habitat << " (" << animalsByHabitat[habitat].size() << ")" << '\n';
            for (const Animal *animal : animalsByHabitat[habitat]) {
                report << "  - " << animal->reportLine() << " | " << animal->socialGroupLabel()
                       << '\n';
            }
            report << '\n';
        }

        std::cout << "Zoo population report written to " << reportPath << "\n";
    } catch (const std::exception &ex) {
        // Catching std::exception here gives us a friendly error message if anything fails.
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}

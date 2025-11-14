# Zookeeper's Challenge Design Report

## Overview
The **Zookeeper's Challenge** application ingests structured records for newly-arrived animals, enriches the data using curated name pools, and produces a population report grouped by habitat. The goal is to demonstrate fluency with C++ control structures, file I/O, data structures, inheritance, and polymorphism while keeping the implementation approachable for future coursework.

## Architecture Highlights
- **Animal Hierarchy** – A polymorphic base `Animal` class encapsulates common attributes (name, ID, sex, age, color, origin, dates). Four derived classes (`Hyena`, `Lion`, `Tiger`, `Bear`) contribute species-specific behavior by overriding `habitatName()` and `socialGroupLabel()`. Each subclass stores a contextual social-group label (clan, pride, ambush, sleuth) to satisfy the "unique attribute" requirement.
- **Data Processing Pipeline** – The program loads reusable name pools from `data/animalNames.txt`, then parses structured arrival descriptions from `data/arrivingAnimals.txt`. The parser extracts age, sex, species, birth season, and other descriptive fields while validating that each record matches the expected format.
- **Supporting Utilities** – Helper functions provide trimming, case normalization, tokenization, and ISO date parsing. `genBirthDay()` converts an age and birth season into a full date, defaulting to the arrival month/day when the season is missing. `genUniqueId()` uses a prefix-based counter map to guarantee species-specific identifiers (e.g., `Hy01`, `Li02`).

## Data Structures
- `std::map<std::string, std::vector<std::string>>` caches available names per species and is consumed sequentially as animals are created.
- `std::vector<std::unique_ptr<Animal>>` owns all instantiated animals, ensuring automatic cleanup and safe polymorphic behavior.
- `std::map<std::string, std::vector<Animal*>>` indexes animals by habitat to simplify report generation and demonstrate working with both owning and non-owning collections.
- `std::map<std::string, int>` tracks ID counters and species counts, fulfilling the requirement to utilize associative containers.

## Output Report
The generated `zooPopulation.txt` file lists each habitat, the total number of occupants, and a detailed line for every animal:
```
Hyena Habitat (2)
  - Hy01; Kamari; birth date 2020-03-15; tan color; female; 70 pounds; from Friguia Park, Tunisia; arrived 2024-03-05 | Clan: Motto Clan
```
The structure mirrors the sample provided in the assignment brief and highlights the assigned name, calculated birth date, origin, and social grouping.

## Error Handling
The application throws descriptive `std::runtime_error` exceptions when encountering malformed rows or inaccessible files. The `main()` routine guards the entire workflow with a `try/catch` block so that any fatal issue results in a readable console message.

## Future Enhancements
Potential improvements include:
- Persisting counters between runs to avoid ID reuse across sessions.
- Refactoring the parser to accommodate additional species or optional fields with minimal changes.
- Exposing command-line flags for custom data directories, enabling easier automation.

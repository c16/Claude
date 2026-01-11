# Diet Tracker App

A comprehensive Flutter diet tracking application with weight monitoring, calorie counting, meal management, and progress visualization.

## Features

### 1. Weight Tracking
- Record weight entries with date and optional notes
- View weight history
- Track weight changes over time
- Delete weight entries

### 2. Meal Catalogue
- Create custom meals with multiple ingredients
- Pre-populated database with common ingredients
- Each ingredient includes:
  - Calories per 100g
  - Protein, carbs, and fat content
- View total calories for each meal
- Expandable meal cards showing all ingredients
- Delete meals

### 3. Daily Calorie Tracking
- Log meals consumed each day
- View total daily calorie intake
- Select different dates to view historical data
- Add optional notes to meal logs
- Delete individual meal logs

### 4. Charts and Analytics
- Weight progress line chart
- Daily calorie intake bar chart
- View data for last 7, 30, or 90 days
- Statistics showing:
  - Average daily calories
  - Maximum daily calories
  - Minimum daily calories

## Technology Stack

- **Framework**: Flutter
- **Database**: SQLite (using sqflite package)
- **Charts**: fl_chart package
- **Date Formatting**: intl package

## Database Schema

The app uses SQLite with the following tables:

1. **weight_entries**: Stores weight measurements
2. **ingredients**: Catalogue of food ingredients with nutritional info
3. **meals**: User-defined meals
4. **meal_ingredients**: Junction table linking meals to ingredients
5. **daily_logs**: Records of meals consumed each day

## Pre-loaded Ingredients

The app comes with 10 common ingredients:
- Chicken Breast
- Brown Rice
- Broccoli
- Olive Oil
- Eggs
- Banana
- Oatmeal
- Milk
- Salmon
- Sweet Potato

## Getting Started

### Prerequisites
- Flutter SDK (3.0.0 or higher)
- Android Studio / Xcode for mobile development
- Dart SDK

### Installation

1. Clone the repository
2. Navigate to the project directory:
   ```
   cd diet_tracker_app
   ```
3. Get Flutter packages:
   ```
   flutter pub get
   ```
4. Run the app:
   ```
   flutter run
   ```

## Usage

### Adding Weight
1. Navigate to the "Weight" tab
2. Tap the + button
3. Enter your weight, select date, and add optional notes
4. Tap "Add" to save

### Creating Meals
1. Navigate to the "Meals" tab
2. Tap the + button
3. Enter meal name and description
4. Add ingredients by selecting from the catalogue and specifying amounts in grams
5. Tap "Save" to create the meal

### Logging Daily Meals
1. Navigate to the "Today" tab (default screen)
2. Tap the + button
3. Select a meal from your catalogue
4. Add optional notes
5. Tap "Log" to record the meal

### Viewing Charts
1. Navigate to the "Charts" tab
2. View weight progress and calorie trends
3. Use the menu to select different time ranges (7, 30, or 90 days)

## File Structure

```
diet_tracker_app/
├── lib/
│   ├── models/
│   │   ├── daily_log.dart
│   │   ├── ingredient.dart
│   │   ├── meal.dart
│   │   └── weight_entry.dart
│   ├── screens/
│   │   ├── charts_screen.dart
│   │   ├── daily_calorie_screen.dart
│   │   ├── home_screen.dart
│   │   ├── meal_catalogue_screen.dart
│   │   └── weight_tracking_screen.dart
│   ├── services/
│   │   └── database_helper.dart
│   └── main.dart
├── android/
├── ios/
└── pubspec.yaml
```

## Features in Detail

### Database Helper
The `DatabaseHelper` class provides methods for:
- Initializing the database with schema
- Seeding default ingredients
- CRUD operations for all entities
- Querying calorie data by date range
- Retrieving meal details with ingredients

### Navigation
Bottom navigation bar with 4 tabs:
- Today (Daily Calorie Tracking)
- Weight (Weight Tracking)
- Meals (Meal Catalogue)
- Charts (Analytics)

## Future Enhancements

Potential features for future versions:
- Export data to CSV
- Meal photos
- Barcode scanner for packaged foods
- Nutritional goals and recommendations
- Water intake tracking
- Exercise tracking
- Cloud sync
- Multiple user profiles

## License

This project is open source and available for personal use.

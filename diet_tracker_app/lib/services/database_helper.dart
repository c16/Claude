import 'package:sqflite/sqflite.dart';
import 'package:path/path.dart';
import '../models/weight_entry.dart';
import '../models/ingredient.dart';
import '../models/meal.dart';
import '../models/daily_log.dart';

class DatabaseHelper {
  static final DatabaseHelper instance = DatabaseHelper._init();
  static Database? _database;

  DatabaseHelper._init();

  Future<Database> get database async {
    if (_database != null) return _database!;
    _database = await _initDB('diet_tracker.db');
    return _database!;
  }

  Future<Database> _initDB(String filePath) async {
    final dbPath = await getDatabasesPath();
    final path = join(dbPath, filePath);

    return await openDatabase(
      path,
      version: 1,
      onCreate: _createDB,
    );
  }

  Future<void> _createDB(Database db, int version) async {
    // Weight entries table
    await db.execute('''
      CREATE TABLE weight_entries (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        weight REAL NOT NULL,
        date TEXT NOT NULL,
        notes TEXT
      )
    ''');

    // Ingredients table
    await db.execute('''
      CREATE TABLE ingredients (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL UNIQUE,
        calories_per_100g REAL NOT NULL,
        protein_per_100g REAL,
        carbs_per_100g REAL,
        fat_per_100g REAL
      )
    ''');

    // Meals table
    await db.execute('''
      CREATE TABLE meals (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        description TEXT
      )
    ''');

    // Meal ingredients junction table
    await db.execute('''
      CREATE TABLE meal_ingredients (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        meal_id INTEGER NOT NULL,
        ingredient_id INTEGER NOT NULL,
        amount_in_grams REAL NOT NULL,
        FOREIGN KEY (meal_id) REFERENCES meals (id) ON DELETE CASCADE,
        FOREIGN KEY (ingredient_id) REFERENCES ingredients (id) ON DELETE CASCADE
      )
    ''');

    // Daily logs table
    await db.execute('''
      CREATE TABLE daily_logs (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        meal_id INTEGER NOT NULL,
        date TEXT NOT NULL,
        notes TEXT,
        FOREIGN KEY (meal_id) REFERENCES meals (id) ON DELETE CASCADE
      )
    ''');

    // Seed some default ingredients
    await _seedDefaultData(db);
  }

  Future<void> _seedDefaultData(Database db) async {
    final ingredients = [
      {'name': 'Chicken Breast', 'calories_per_100g': 165, 'protein_per_100g': 31, 'carbs_per_100g': 0, 'fat_per_100g': 3.6},
      {'name': 'Brown Rice', 'calories_per_100g': 112, 'protein_per_100g': 2.6, 'carbs_per_100g': 24, 'fat_per_100g': 0.9},
      {'name': 'Broccoli', 'calories_per_100g': 34, 'protein_per_100g': 2.8, 'carbs_per_100g': 7, 'fat_per_100g': 0.4},
      {'name': 'Olive Oil', 'calories_per_100g': 884, 'protein_per_100g': 0, 'carbs_per_100g': 0, 'fat_per_100g': 100},
      {'name': 'Eggs', 'calories_per_100g': 155, 'protein_per_100g': 13, 'carbs_per_100g': 1.1, 'fat_per_100g': 11},
      {'name': 'Banana', 'calories_per_100g': 89, 'protein_per_100g': 1.1, 'carbs_per_100g': 23, 'fat_per_100g': 0.3},
      {'name': 'Oatmeal', 'calories_per_100g': 389, 'protein_per_100g': 17, 'carbs_per_100g': 66, 'fat_per_100g': 6.9},
      {'name': 'Milk', 'calories_per_100g': 42, 'protein_per_100g': 3.4, 'carbs_per_100g': 5, 'fat_per_100g': 1},
      {'name': 'Salmon', 'calories_per_100g': 208, 'protein_per_100g': 20, 'carbs_per_100g': 0, 'fat_per_100g': 13},
      {'name': 'Sweet Potato', 'calories_per_100g': 86, 'protein_per_100g': 1.6, 'carbs_per_100g': 20, 'fat_per_100g': 0.1},
    ];

    for (var ingredient in ingredients) {
      await db.insert('ingredients', ingredient);
    }
  }

  // Weight Entry operations
  Future<int> insertWeightEntry(WeightEntry entry) async {
    final db = await database;
    return await db.insert('weight_entries', entry.toMap());
  }

  Future<List<WeightEntry>> getWeightEntries({int? limit}) async {
    final db = await database;
    final result = await db.query(
      'weight_entries',
      orderBy: 'date DESC',
      limit: limit,
    );
    return result.map((map) => WeightEntry.fromMap(map)).toList();
  }

  Future<int> deleteWeightEntry(int id) async {
    final db = await database;
    return await db.delete('weight_entries', where: 'id = ?', whereArgs: [id]);
  }

  // Ingredient operations
  Future<int> insertIngredient(Ingredient ingredient) async {
    final db = await database;
    return await db.insert('ingredients', ingredient.toMap());
  }

  Future<List<Ingredient>> getAllIngredients() async {
    final db = await database;
    final result = await db.query('ingredients', orderBy: 'name ASC');
    return result.map((map) => Ingredient.fromMap(map)).toList();
  }

  Future<Ingredient?> getIngredient(int id) async {
    final db = await database;
    final result = await db.query('ingredients', where: 'id = ?', whereArgs: [id]);
    if (result.isEmpty) return null;
    return Ingredient.fromMap(result.first);
  }

  // Meal operations
  Future<int> insertMeal(Meal meal) async {
    final db = await database;
    final mealId = await db.insert('meals', meal.toMap());

    for (var ingredient in meal.ingredients) {
      await db.insert('meal_ingredients', {
        'meal_id': mealId,
        'ingredient_id': ingredient.ingredientId,
        'amount_in_grams': ingredient.amountInGrams,
      });
    }

    return mealId;
  }

  Future<List<Meal>> getAllMeals() async {
    final db = await database;
    final result = await db.query('meals', orderBy: 'name ASC');

    List<Meal> meals = [];
    for (var mealMap in result) {
      final ingredients = await getMealIngredients(mealMap['id'] as int);
      meals.add(Meal(
        id: mealMap['id'] as int,
        name: mealMap['name'] as String,
        description: mealMap['description'] as String?,
        ingredients: ingredients,
      ));
    }

    return meals;
  }

  Future<Meal?> getMeal(int id) async {
    final db = await database;
    final result = await db.query('meals', where: 'id = ?', whereArgs: [id]);
    if (result.isEmpty) return null;

    final ingredients = await getMealIngredients(id);
    return Meal(
      id: result.first['id'] as int,
      name: result.first['name'] as String,
      description: result.first['description'] as String?,
      ingredients: ingredients,
    );
  }

  Future<List<MealIngredient>> getMealIngredients(int mealId) async {
    final db = await database;
    final result = await db.rawQuery('''
      SELECT mi.id, mi.meal_id, mi.ingredient_id, mi.amount_in_grams,
             i.name as ingredient_name, i.calories_per_100g
      FROM meal_ingredients mi
      INNER JOIN ingredients i ON mi.ingredient_id = i.id
      WHERE mi.meal_id = ?
    ''', [mealId]);

    return result.map((map) => MealIngredient.fromMap(map)).toList();
  }

  Future<int> deleteMeal(int id) async {
    final db = await database;
    return await db.delete('meals', where: 'id = ?', whereArgs: [id]);
  }

  // Daily log operations
  Future<int> insertDailyLog(DailyLog log) async {
    final db = await database;
    return await db.insert('daily_logs', log.toMap());
  }

  Future<List<DailyLog>> getDailyLogs(DateTime date) async {
    final db = await database;
    final startOfDay = DateTime(date.year, date.month, date.day);
    final endOfDay = startOfDay.add(const Duration(days: 1));

    final result = await db.rawQuery('''
      SELECT dl.id, dl.meal_id, dl.date, dl.notes,
             m.name as meal_name, SUM(mi.amount_in_grams / 100 * i.calories_per_100g) as calories
      FROM daily_logs dl
      INNER JOIN meals m ON dl.meal_id = m.id
      INNER JOIN meal_ingredients mi ON m.id = mi.meal_id
      INNER JOIN ingredients i ON mi.ingredient_id = i.id
      WHERE dl.date >= ? AND dl.date < ?
      GROUP BY dl.id
      ORDER BY dl.date DESC
    ''', [startOfDay.toIso8601String(), endOfDay.toIso8601String()]);

    return result.map((map) => DailyLog.fromMap(map)).toList();
  }

  Future<Map<String, double>> getCaloriesByDateRange(DateTime start, DateTime end) async {
    final db = await database;
    final result = await db.rawQuery('''
      SELECT DATE(dl.date) as date, SUM(mi.amount_in_grams / 100 * i.calories_per_100g) as total_calories
      FROM daily_logs dl
      INNER JOIN meal_ingredients mi ON dl.meal_id = mi.meal_id
      INNER JOIN ingredients i ON mi.ingredient_id = i.id
      WHERE dl.date >= ? AND dl.date < ?
      GROUP BY DATE(dl.date)
      ORDER BY date ASC
    ''', [start.toIso8601String(), end.toIso8601String()]);

    Map<String, double> caloriesByDate = {};
    for (var row in result) {
      caloriesByDate[row['date'] as String] = row['total_calories'] as double;
    }
    return caloriesByDate;
  }

  Future<int> deleteDailyLog(int id) async {
    final db = await database;
    return await db.delete('daily_logs', where: 'id = ?', whereArgs: [id]);
  }

  Future<void> close() async {
    final db = await database;
    await db.close();
  }
}

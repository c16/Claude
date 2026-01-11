class Meal {
  final int? id;
  final String name;
  final String? description;
  final List<MealIngredient> ingredients;

  Meal({
    this.id,
    required this.name,
    this.description,
    this.ingredients = const [],
  });

  double get totalCalories {
    return ingredients.fold(0, (sum, ing) => sum + ing.calories);
  }

  Map<String, dynamic> toMap() {
    return {
      'id': id,
      'name': name,
      'description': description,
    };
  }

  factory Meal.fromMap(Map<String, dynamic> map) {
    return Meal(
      id: map['id'],
      name: map['name'],
      description: map['description'],
    );
  }
}

class MealIngredient {
  final int? id;
  final int mealId;
  final int ingredientId;
  final String ingredientName;
  final double amountInGrams;
  final double caloriesPer100g;

  MealIngredient({
    this.id,
    required this.mealId,
    required this.ingredientId,
    required this.ingredientName,
    required this.amountInGrams,
    required this.caloriesPer100g,
  });

  double get calories => (amountInGrams / 100) * caloriesPer100g;

  Map<String, dynamic> toMap() {
    return {
      'id': id,
      'meal_id': mealId,
      'ingredient_id': ingredientId,
      'amount_in_grams': amountInGrams,
    };
  }

  factory MealIngredient.fromMap(Map<String, dynamic> map) {
    return MealIngredient(
      id: map['id'],
      mealId: map['meal_id'],
      ingredientId: map['ingredient_id'],
      ingredientName: map['ingredient_name'],
      amountInGrams: map['amount_in_grams'],
      caloriesPer100g: map['calories_per_100g'],
    );
  }
}

class Ingredient {
  final int? id;
  final String name;
  final double caloriesPer100g;
  final double? proteinPer100g;
  final double? carbsPer100g;
  final double? fatPer100g;

  Ingredient({
    this.id,
    required this.name,
    required this.caloriesPer100g,
    this.proteinPer100g,
    this.carbsPer100g,
    this.fatPer100g,
  });

  Map<String, dynamic> toMap() {
    return {
      'id': id,
      'name': name,
      'calories_per_100g': caloriesPer100g,
      'protein_per_100g': proteinPer100g,
      'carbs_per_100g': carbsPer100g,
      'fat_per_100g': fatPer100g,
    };
  }

  factory Ingredient.fromMap(Map<String, dynamic> map) {
    return Ingredient(
      id: map['id'],
      name: map['name'],
      caloriesPer100g: map['calories_per_100g'],
      proteinPer100g: map['protein_per_100g'],
      carbsPer100g: map['carbs_per_100g'],
      fatPer100g: map['fat_per_100g'],
    );
  }
}

class DailyLog {
  final int? id;
  final int mealId;
  final String mealName;
  final DateTime date;
  final double calories;
  final String? notes;

  DailyLog({
    this.id,
    required this.mealId,
    required this.mealName,
    required this.date,
    required this.calories,
    this.notes,
  });

  Map<String, dynamic> toMap() {
    return {
      'id': id,
      'meal_id': mealId,
      'date': date.toIso8601String(),
      'notes': notes,
    };
  }

  factory DailyLog.fromMap(Map<String, dynamic> map) {
    return DailyLog(
      id: map['id'],
      mealId: map['meal_id'],
      mealName: map['meal_name'],
      date: DateTime.parse(map['date']),
      calories: map['calories'],
      notes: map['notes'],
    );
  }
}

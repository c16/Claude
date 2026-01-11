class WeightEntry {
  final int? id;
  final double weight;
  final DateTime date;
  final String? notes;

  WeightEntry({
    this.id,
    required this.weight,
    required this.date,
    this.notes,
  });

  Map<String, dynamic> toMap() {
    return {
      'id': id,
      'weight': weight,
      'date': date.toIso8601String(),
      'notes': notes,
    };
  }

  factory WeightEntry.fromMap(Map<String, dynamic> map) {
    return WeightEntry(
      id: map['id'],
      weight: map['weight'],
      date: DateTime.parse(map['date']),
      notes: map['notes'],
    );
  }
}

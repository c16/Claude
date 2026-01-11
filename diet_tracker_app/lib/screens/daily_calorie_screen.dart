import 'package:flutter/material.dart';
import 'package:intl/intl.dart';
import '../models/daily_log.dart';
import '../models/meal.dart';
import '../services/database_helper.dart';

class DailyCalorieScreen extends StatefulWidget {
  const DailyCalorieScreen({super.key});

  @override
  State<DailyCalorieScreen> createState() => _DailyCalorieScreenState();
}

class _DailyCalorieScreenState extends State<DailyCalorieScreen> {
  final DatabaseHelper _dbHelper = DatabaseHelper.instance;
  List<DailyLog> _logs = [];
  DateTime _selectedDate = DateTime.now();
  bool _isLoading = true;

  @override
  void initState() {
    super.initState();
    _loadLogs();
  }

  Future<void> _loadLogs() async {
    setState(() => _isLoading = true);
    final logs = await _dbHelper.getDailyLogs(_selectedDate);
    setState(() {
      _logs = logs;
      _isLoading = false;
    });
  }

  double get _totalCalories {
    return _logs.fold(0, (sum, log) => sum + log.calories);
  }

  Future<void> _showAddMealLogDialog() async {
    final meals = await _dbHelper.getAllMeals();
    if (meals.isEmpty) {
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          const SnackBar(content: Text('Please create meals first in the Meals tab')),
        );
      }
      return;
    }

    Meal? selectedMeal;
    final notesController = TextEditingController();

    await showDialog(
      context: context,
      builder: (context) => StatefulBuilder(
        builder: (context, setDialogState) => AlertDialog(
          title: const Text('Log Meal'),
          content: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              DropdownButtonFormField<Meal>(
                decoration: const InputDecoration(
                  labelText: 'Select Meal',
                  border: OutlineInputBorder(),
                ),
                items: meals.map((meal) {
                  return DropdownMenuItem(
                    value: meal,
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Text(meal.name),
                        Text(
                          '${meal.totalCalories.toStringAsFixed(0)} cal',
                          style: const TextStyle(fontSize: 12, color: Colors.grey),
                        ),
                      ],
                    ),
                  );
                }).toList(),
                onChanged: (value) {
                  setDialogState(() => selectedMeal = value);
                },
              ),
              const SizedBox(height: 16),
              TextField(
                controller: notesController,
                decoration: const InputDecoration(
                  labelText: 'Notes (optional)',
                  border: OutlineInputBorder(),
                ),
                maxLines: 2,
              ),
            ],
          ),
          actions: [
            TextButton(
              onPressed: () => Navigator.pop(context),
              child: const Text('Cancel'),
            ),
            ElevatedButton(
              onPressed: () async {
                if (selectedMeal != null) {
                  final log = DailyLog(
                    mealId: selectedMeal!.id!,
                    mealName: selectedMeal!.name,
                    date: _selectedDate,
                    calories: selectedMeal!.totalCalories,
                    notes: notesController.text.isEmpty ? null : notesController.text,
                  );
                  await _dbHelper.insertDailyLog(log);
                  if (context.mounted) Navigator.pop(context);
                  _loadLogs();
                }
              },
              child: const Text('Log'),
            ),
          ],
        ),
      ),
    );
  }

  Future<void> _selectDate() async {
    final date = await showDatePicker(
      context: context,
      initialDate: _selectedDate,
      firstDate: DateTime(2020),
      lastDate: DateTime.now(),
    );
    if (date != null) {
      setState(() => _selectedDate = date);
      _loadLogs();
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Daily Calories'),
        actions: [
          IconButton(
            icon: const Icon(Icons.calendar_today),
            onPressed: _selectDate,
          ),
        ],
      ),
      body: Column(
        children: [
          Card(
            margin: const EdgeInsets.all(16),
            color: Colors.green.shade50,
            child: Padding(
              padding: const EdgeInsets.all(16),
              child: Column(
                children: [
                  Text(
                    DateFormat('EEEE, MMMM d, y').format(_selectedDate),
                    style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
                  ),
                  const SizedBox(height: 8),
                  Text(
                    '${_totalCalories.toStringAsFixed(0)} calories',
                    style: TextStyle(
                      fontSize: 32,
                      fontWeight: FontWeight.bold,
                      color: Colors.green.shade700,
                    ),
                  ),
                  const SizedBox(height: 4),
                  Text(
                    '${_logs.length} meal${_logs.length != 1 ? 's' : ''} logged',
                    style: const TextStyle(color: Colors.grey),
                  ),
                ],
              ),
            ),
          ),
          Expanded(
            child: _isLoading
                ? const Center(child: CircularProgressIndicator())
                : _logs.isEmpty
                    ? const Center(
                        child: Text('No meals logged for this day.\nTap + to add one.'),
                      )
                    : ListView.builder(
                        itemCount: _logs.length,
                        itemBuilder: (context, index) {
                          final log = _logs[index];
                          return Card(
                            margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
                            child: ListTile(
                              leading: CircleAvatar(
                                child: Text('${log.calories.toStringAsFixed(0)}'),
                              ),
                              title: Text(
                                log.mealName,
                                style: const TextStyle(fontWeight: FontWeight.bold),
                              ),
                              subtitle: Column(
                                crossAxisAlignment: CrossAxisAlignment.start,
                                children: [
                                  Text('${log.calories.toStringAsFixed(0)} calories'),
                                  if (log.notes != null) Text(log.notes!),
                                ],
                              ),
                              trailing: IconButton(
                                icon: const Icon(Icons.delete, color: Colors.red),
                                onPressed: () async {
                                  await _dbHelper.deleteDailyLog(log.id!);
                                  _loadLogs();
                                },
                              ),
                            ),
                          );
                        },
                      ),
          ),
        ],
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: _showAddMealLogDialog,
        child: const Icon(Icons.add),
      ),
    );
  }
}

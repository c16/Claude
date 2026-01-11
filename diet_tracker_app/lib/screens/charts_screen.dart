import 'package:flutter/material.dart';
import 'package:fl_chart/fl_chart.dart';
import 'package:intl/intl.dart';
import '../models/weight_entry.dart';
import '../services/database_helper.dart';

class ChartsScreen extends StatefulWidget {
  const ChartsScreen({super.key});

  @override
  State<ChartsScreen> createState() => _ChartsScreenState();
}

class _ChartsScreenState extends State<ChartsScreen> {
  final DatabaseHelper _dbHelper = DatabaseHelper.instance;
  List<WeightEntry> _weightEntries = [];
  Map<String, double> _calorieData = {};
  bool _isLoading = true;
  int _selectedDays = 30;

  @override
  void initState() {
    super.initState();
    _loadData();
  }

  Future<void> _loadData() async {
    setState(() => _isLoading = true);

    final weightEntries = await _dbHelper.getWeightEntries(limit: _selectedDays);
    final endDate = DateTime.now();
    final startDate = endDate.subtract(Duration(days: _selectedDays));
    final calorieData = await _dbHelper.getCaloriesByDateRange(startDate, endDate);

    setState(() {
      _weightEntries = weightEntries.reversed.toList();
      _calorieData = calorieData;
      _isLoading = false;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Charts'),
        actions: [
          PopupMenuButton<int>(
            initialValue: _selectedDays,
            onSelected: (days) {
              setState(() => _selectedDays = days);
              _loadData();
            },
            itemBuilder: (context) => [
              const PopupMenuItem(value: 7, child: Text('Last 7 days')),
              const PopupMenuItem(value: 30, child: Text('Last 30 days')),
              const PopupMenuItem(value: 90, child: Text('Last 90 days')),
            ],
          ),
        ],
      ),
      body: _isLoading
          ? const Center(child: CircularProgressIndicator())
          : SingleChildScrollView(
              padding: const EdgeInsets.all(16),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  const Text(
                    'Weight Progress',
                    style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
                  ),
                  const SizedBox(height: 16),
                  _weightEntries.isEmpty
                      ? const Card(
                          child: Padding(
                            padding: EdgeInsets.all(32),
                            child: Center(child: Text('No weight data available')),
                          ),
                        )
                      : Card(
                          child: Padding(
                            padding: const EdgeInsets.all(16),
                            child: SizedBox(
                              height: 250,
                              child: LineChart(
                                LineChartData(
                                  gridData: const FlGridData(show: true),
                                  titlesData: FlTitlesData(
                                    leftTitles: AxisTitles(
                                      sideTitles: SideTitles(
                                        showTitles: true,
                                        reservedSize: 40,
                                        getTitlesWidget: (value, meta) {
                                          return Text(
                                            value.toStringAsFixed(1),
                                            style: const TextStyle(fontSize: 10),
                                          );
                                        },
                                      ),
                                    ),
                                    rightTitles: const AxisTitles(
                                      sideTitles: SideTitles(showTitles: false),
                                    ),
                                    topTitles: const AxisTitles(
                                      sideTitles: SideTitles(showTitles: false),
                                    ),
                                    bottomTitles: AxisTitles(
                                      sideTitles: SideTitles(
                                        showTitles: true,
                                        reservedSize: 30,
                                        getTitlesWidget: (value, meta) {
                                          if (value.toInt() >= 0 && value.toInt() < _weightEntries.length) {
                                            final date = _weightEntries[value.toInt()].date;
                                            return Padding(
                                              padding: const EdgeInsets.only(top: 8),
                                              child: Text(
                                                DateFormat('M/d').format(date),
                                                style: const TextStyle(fontSize: 10),
                                              ),
                                            );
                                          }
                                          return const Text('');
                                        },
                                      ),
                                    ),
                                  ),
                                  borderData: FlBorderData(show: true),
                                  lineBarsData: [
                                    LineChartBarData(
                                      spots: _weightEntries
                                          .asMap()
                                          .entries
                                          .map((entry) => FlSpot(
                                                entry.key.toDouble(),
                                                entry.value.weight,
                                              ))
                                          .toList(),
                                      isCurved: true,
                                      color: Colors.blue,
                                      barWidth: 3,
                                      dotData: const FlDotData(show: true),
                                      belowBarData: BarAreaData(
                                        show: true,
                                        color: Colors.blue.withOpacity(0.1),
                                      ),
                                    ),
                                  ],
                                ),
                              ),
                            ),
                          ),
                        ),
                  const SizedBox(height: 32),
                  const Text(
                    'Daily Calorie Intake',
                    style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
                  ),
                  const SizedBox(height: 16),
                  _calorieData.isEmpty
                      ? const Card(
                          child: Padding(
                            padding: EdgeInsets.all(32),
                            child: Center(child: Text('No calorie data available')),
                          ),
                        )
                      : Card(
                          child: Padding(
                            padding: const EdgeInsets.all(16),
                            child: SizedBox(
                              height: 250,
                              child: BarChart(
                                BarChartData(
                                  alignment: BarChartAlignment.spaceAround,
                                  gridData: const FlGridData(show: true),
                                  titlesData: FlTitlesData(
                                    leftTitles: AxisTitles(
                                      sideTitles: SideTitles(
                                        showTitles: true,
                                        reservedSize: 40,
                                        getTitlesWidget: (value, meta) {
                                          return Text(
                                            value.toInt().toString(),
                                            style: const TextStyle(fontSize: 10),
                                          );
                                        },
                                      ),
                                    ),
                                    rightTitles: const AxisTitles(
                                      sideTitles: SideTitles(showTitles: false),
                                    ),
                                    topTitles: const AxisTitles(
                                      sideTitles: SideTitles(showTitles: false),
                                    ),
                                    bottomTitles: AxisTitles(
                                      sideTitles: SideTitles(
                                        showTitles: true,
                                        reservedSize: 30,
                                        getTitlesWidget: (value, meta) {
                                          final dates = _calorieData.keys.toList();
                                          if (value.toInt() >= 0 && value.toInt() < dates.length) {
                                            final dateStr = dates[value.toInt()];
                                            final date = DateTime.parse(dateStr);
                                            return Padding(
                                              padding: const EdgeInsets.only(top: 8),
                                              child: Text(
                                                DateFormat('M/d').format(date),
                                                style: const TextStyle(fontSize: 10),
                                              ),
                                            );
                                          }
                                          return const Text('');
                                        },
                                      ),
                                    ),
                                  ),
                                  borderData: FlBorderData(show: true),
                                  barGroups: _calorieData.entries
                                      .toList()
                                      .asMap()
                                      .entries
                                      .map((entry) {
                                    return BarChartGroupData(
                                      x: entry.key,
                                      barRods: [
                                        BarChartRodData(
                                          toY: entry.value.value,
                                          color: Colors.green,
                                          width: 16,
                                          borderRadius: const BorderRadius.only(
                                            topLeft: Radius.circular(6),
                                            topRight: Radius.circular(6),
                                          ),
                                        ),
                                      ],
                                    );
                                  }).toList(),
                                ),
                              ),
                            ),
                          ),
                        ),
                  const SizedBox(height: 16),
                  if (_calorieData.isNotEmpty)
                    Card(
                      child: Padding(
                        padding: const EdgeInsets.all(16),
                        child: Column(
                          children: [
                            Row(
                              mainAxisAlignment: MainAxisAlignment.spaceAround,
                              children: [
                                Column(
                                  children: [
                                    const Text('Average', style: TextStyle(color: Colors.grey)),
                                    const SizedBox(height: 4),
                                    Text(
                                      '${(_calorieData.values.reduce((a, b) => a + b) / _calorieData.length).toStringAsFixed(0)} cal',
                                      style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
                                    ),
                                  ],
                                ),
                                Column(
                                  children: [
                                    const Text('Max', style: TextStyle(color: Colors.grey)),
                                    const SizedBox(height: 4),
                                    Text(
                                      '${_calorieData.values.reduce((a, b) => a > b ? a : b).toStringAsFixed(0)} cal',
                                      style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
                                    ),
                                  ],
                                ),
                                Column(
                                  children: [
                                    const Text('Min', style: TextStyle(color: Colors.grey)),
                                    const SizedBox(height: 4),
                                    Text(
                                      '${_calorieData.values.reduce((a, b) => a < b ? a : b).toStringAsFixed(0)} cal',
                                      style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
                                    ),
                                  ],
                                ),
                              ],
                            ),
                          ],
                        ),
                      ),
                    ),
                ],
              ),
            ),
    );
  }
}

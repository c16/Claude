import 'package:flutter/material.dart';
import 'weight_tracking_screen.dart';
import 'meal_catalogue_screen.dart';
import 'daily_calorie_screen.dart';
import 'charts_screen.dart';

class HomeScreen extends StatefulWidget {
  const HomeScreen({super.key});

  @override
  State<HomeScreen> createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  int _selectedIndex = 0;

  final List<Widget> _screens = [
    const DailyCalorieScreen(),
    const WeightTrackingScreen(),
    const MealCatalogueScreen(),
    const ChartsScreen(),
  ];

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: _screens[_selectedIndex],
      bottomNavigationBar: NavigationBar(
        selectedIndex: _selectedIndex,
        onDestinationSelected: (index) {
          setState(() {
            _selectedIndex = index;
          });
        },
        destinations: const [
          NavigationDestination(
            icon: Icon(Icons.restaurant),
            label: 'Today',
          ),
          NavigationDestination(
            icon: Icon(Icons.monitor_weight),
            label: 'Weight',
          ),
          NavigationDestination(
            icon: Icon(Icons.menu_book),
            label: 'Meals',
          ),
          NavigationDestination(
            icon: Icon(Icons.analytics),
            label: 'Charts',
          ),
        ],
      ),
    );
  }
}

import 'package:flutter/material.dart';
import 'screens/home_screen.dart';

void main() {
  runApp(const DietTrackerApp());
}

class DietTrackerApp extends StatelessWidget {
  const DietTrackerApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Diet Tracker',
      theme: ThemeData(
        primarySwatch: Colors.green,
        useMaterial3: true,
      ),
      home: const HomeScreen(),
    );
  }
}

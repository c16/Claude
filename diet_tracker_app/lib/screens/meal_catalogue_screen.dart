import 'package:flutter/material.dart';
import '../models/meal.dart';
import '../models/ingredient.dart';
import '../services/database_helper.dart';

class MealCatalogueScreen extends StatefulWidget {
  const MealCatalogueScreen({super.key});

  @override
  State<MealCatalogueScreen> createState() => _MealCatalogueScreenState();
}

class _MealCatalogueScreenState extends State<MealCatalogueScreen> {
  final DatabaseHelper _dbHelper = DatabaseHelper.instance;
  List<Meal> _meals = [];
  bool _isLoading = true;

  @override
  void initState() {
    super.initState();
    _loadMeals();
  }

  Future<void> _loadMeals() async {
    setState(() => _isLoading = true);
    final meals = await _dbHelper.getAllMeals();
    setState(() {
      _meals = meals;
      _isLoading = false;
    });
  }

  Future<void> _showAddMealDialog() async {
    final nameController = TextEditingController();
    final descriptionController = TextEditingController();
    final allIngredients = await _dbHelper.getAllIngredients();
    List<MealIngredient> selectedIngredients = [];

    await showDialog(
      context: context,
      builder: (context) => StatefulBuilder(
        builder: (context, setDialogState) => AlertDialog(
          title: const Text('Add New Meal'),
          content: SingleChildScrollView(
            child: Column(
              mainAxisSize: MainAxisSize.min,
              children: [
                TextField(
                  controller: nameController,
                  decoration: const InputDecoration(
                    labelText: 'Meal Name',
                    border: OutlineInputBorder(),
                  ),
                ),
                const SizedBox(height: 16),
                TextField(
                  controller: descriptionController,
                  decoration: const InputDecoration(
                    labelText: 'Description (optional)',
                    border: OutlineInputBorder(),
                  ),
                  maxLines: 2,
                ),
                const SizedBox(height: 16),
                const Text('Ingredients:', style: TextStyle(fontWeight: FontWeight.bold)),
                const SizedBox(height: 8),
                ...selectedIngredients.map((mi) => ListTile(
                      title: Text(mi.ingredientName),
                      subtitle: Text('${mi.amountInGrams}g'),
                      trailing: IconButton(
                        icon: const Icon(Icons.delete, color: Colors.red),
                        onPressed: () {
                          setDialogState(() {
                            selectedIngredients.removeWhere((i) => i.ingredientId == mi.ingredientId);
                          });
                        },
                      ),
                    )),
                ElevatedButton.icon(
                  onPressed: () async {
                    await _showAddIngredientToMealDialog(
                      allIngredients,
                      (ingredient, amount) {
                        setDialogState(() {
                          selectedIngredients.add(MealIngredient(
                            mealId: 0,
                            ingredientId: ingredient.id!,
                            ingredientName: ingredient.name,
                            amountInGrams: amount,
                            caloriesPer100g: ingredient.caloriesPer100g,
                          ));
                        });
                      },
                    );
                  },
                  icon: const Icon(Icons.add),
                  label: const Text('Add Ingredient'),
                ),
              ],
            ),
          ),
          actions: [
            TextButton(
              onPressed: () => Navigator.pop(context),
              child: const Text('Cancel'),
            ),
            ElevatedButton(
              onPressed: () async {
                if (nameController.text.isNotEmpty && selectedIngredients.isNotEmpty) {
                  final meal = Meal(
                    name: nameController.text,
                    description: descriptionController.text.isEmpty ? null : descriptionController.text,
                    ingredients: selectedIngredients,
                  );
                  await _dbHelper.insertMeal(meal);
                  if (context.mounted) Navigator.pop(context);
                  _loadMeals();
                }
              },
              child: const Text('Save'),
            ),
          ],
        ),
      ),
    );
  }

  Future<void> _showAddIngredientToMealDialog(
    List<Ingredient> allIngredients,
    Function(Ingredient, double) onAdd,
  ) async {
    Ingredient? selectedIngredient;
    final amountController = TextEditingController();

    await showDialog(
      context: context,
      builder: (context) => StatefulBuilder(
        builder: (context, setDialogState) => AlertDialog(
          title: const Text('Add Ingredient'),
          content: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              DropdownButtonFormField<Ingredient>(
                decoration: const InputDecoration(
                  labelText: 'Select Ingredient',
                  border: OutlineInputBorder(),
                ),
                items: allIngredients.map((ingredient) {
                  return DropdownMenuItem(
                    value: ingredient,
                    child: Text(ingredient.name),
                  );
                }).toList(),
                onChanged: (value) {
                  setDialogState(() => selectedIngredient = value);
                },
              ),
              const SizedBox(height: 16),
              TextField(
                controller: amountController,
                decoration: const InputDecoration(
                  labelText: 'Amount (grams)',
                  border: OutlineInputBorder(),
                ),
                keyboardType: const TextInputType.numberWithOptions(decimal: true),
              ),
            ],
          ),
          actions: [
            TextButton(
              onPressed: () => Navigator.pop(context),
              child: const Text('Cancel'),
            ),
            ElevatedButton(
              onPressed: () {
                final amount = double.tryParse(amountController.text);
                if (selectedIngredient != null && amount != null) {
                  onAdd(selectedIngredient!, amount);
                  Navigator.pop(context);
                }
              },
              child: const Text('Add'),
            ),
          ],
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Meal Catalogue'),
      ),
      body: _isLoading
          ? const Center(child: CircularProgressIndicator())
          : _meals.isEmpty
              ? const Center(
                  child: Text('No meals yet.\nTap + to create one.'),
                )
              : ListView.builder(
                  itemCount: _meals.length,
                  itemBuilder: (context, index) {
                    final meal = _meals[index];
                    return Card(
                      margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
                      child: ExpansionTile(
                        title: Text(
                          meal.name,
                          style: const TextStyle(fontWeight: FontWeight.bold),
                        ),
                        subtitle: Text(
                          '${meal.totalCalories.toStringAsFixed(0)} calories',
                          style: const TextStyle(color: Colors.green),
                        ),
                        children: [
                          if (meal.description != null)
                            Padding(
                              padding: const EdgeInsets.all(16),
                              child: Text(meal.description!),
                            ),
                          const Divider(),
                          ...meal.ingredients.map((ingredient) => ListTile(
                                title: Text(ingredient.ingredientName),
                                trailing: Text(
                                  '${ingredient.amountInGrams}g (${ingredient.calories.toStringAsFixed(0)} cal)',
                                ),
                              )),
                          ButtonBar(
                            children: [
                              TextButton.icon(
                                onPressed: () async {
                                  await _dbHelper.deleteMeal(meal.id!);
                                  _loadMeals();
                                },
                                icon: const Icon(Icons.delete, color: Colors.red),
                                label: const Text('Delete', style: TextStyle(color: Colors.red)),
                              ),
                            ],
                          ),
                        ],
                      ),
                    );
                  },
                ),
      floatingActionButton: FloatingActionButton(
        onPressed: _showAddMealDialog,
        child: const Icon(Icons.add),
      ),
    );
  }
}

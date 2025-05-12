def calculate_bmi(weight, height_cm):
    height_m = height_cm / 100
    bmi = weight / (height_m ** 2)
    return round(bmi, 1)

def evaluate_hypertension(sys, dia, symptoms):
    hypertension_risk = False
    if sys >= 140 or dia >= 90:
        hypertension_risk = True
        print("Diagnosis: You have Hypertension (High BP).")
        if symptoms['chest_pain'] == 'yes':
            print("Chest pain reported. Seek medical attention.")
        if symptoms['headache'] == 'yes':
            print(" Frequent headaches indicate vascular stress.")
        print(" Suggestion: Reduce salt intake, check BP regularly, consult cardiologist.\n")

    elif 120 <= sys < 140 or 80 <= dia < 90:
        hypertension_risk = True
        print("Diagnosis: Pre-hypertension stage.")
        print("Suggestion: Manage with lifestyle changes. Monitor BP closely.\n")

    else:
        print("Blood pressure is within normal range.\n")
    return hypertension_risk

def general_symptom_check(diabetes_risk, hypertension_risk, symptoms):
    mild_warning = False
    if symptoms['frequent_thirst'] == 'yes' or symptoms['frequent_urination'] == 'yes' or symptoms['blurred_vision'] == 'yes' or symptoms['fatigue'] == 'yes':
        mild_warning = True
        print("Mild symptoms detected despite normal readings.")
        print("Suggestion: Repeat tests in 2 weeks. Monitor your condition.\n")

    elif symptoms['chest_pain'] == 'yes' or symptoms['headache'] == 'yes':
        mild_warning = True
        print("Chest pain/headache detected without hypertension.")
        print("Suggestion: Could be stress-related. Consider ECG or doctor visit.\n")

    else:
        print("You have no mild symptoms. That's great!")

    return mild_warning

def evaluate_diabetes_risk_score(fasting_sugar, post_meal_sugar, history_diabetes, symptoms):
    score = 0
    if fasting_sugar >= 126:
        score += 2
    if post_meal_sugar >= 200:
        score += 2
    if history_diabetes == 'yes':
        score += 1
    count = len([value for value in symptoms.values() if value == 'yes'])
    if count > 0:
        score += 1

    if score >= 4:
        return "High"
    elif score >= 2:
        return "Moderate"
    else:
        return "Low"


def evaluate_diabetes(fasting, post_meal, history, symptoms):
    risk_level = evaluate_diabetes_risk_score(fasting, post_meal, history, symptoms)
    print(f" Diabetes Risk Level: {risk_level}")
    count = len([value for value in symptoms.values() if value == 'yes'])
    diabetes_risk = False

    if fasting >= 126 or post_meal >= 200:
        diabetes_risk = True
        print(" Diagnosis: You are likely diabetic.")
        if history == 'yes':
            print(" Note: History of diabetes present. Levels are critically high.")
        else:
            print(" Warning: Diabetic levels detected with no prior history.")
        print(" Suggestion: Consult a diabetologist. Begin sugar control therapy.\n")

    elif 100 <= fasting < 126 or 140 <= post_meal < 200:
        diabetes_risk = True
        print(" Diagnosis: Pre-diabetic stage.")
        print(" Suggestion: Control diet, exercise, and monitor monthly.\n")
    
    elif count > 0 and (fasting > 110 or post_meal > 160):
        diabetes_risk = True
        print(" Diagnosis: Symptoms with elevated sugar levels suggest early diabetes.")
        print(" Suggestion: Perform HbA1c test. Visit a physician.\n")

    else:
        print(" Blood sugar levels are within safe range.\n")
    
    return diabetes_risk


def final_summary(age, diabetes_risk, hypertension_risk, mild_warning):
    if not diabetes_risk and not hypertension_risk and not mild_warning:
        print(" You are in good health based on provided inputs.")
        if age > 45:
            print(" Age over 45: Regular checkups are advised.\n")
        else:
            print(" Keep up the healthy lifestyle!\n")
    print("======= END OF REPORT =======")

def main():
    print("===============================================")
    print("      ADVANCED MEDICAL EXPERT SYSTEM           ")
    print(" Focus Areas: Diabetes, Hypertension, General Risk")
    print("===============================================\n")

    age = int(input("Enter your age: "))
    weight = float(input("Enter your weight (in kg): "))
    height = float(input("Enter your height (in cm): "))

    bmi = calculate_bmi(weight, height)
    print(f" Your BMI is: {bmi} - ", end="")
    if bmi < 18.5:
        print("Underweight")
    elif 18.5 <= bmi < 24.9:
        print("Normal (Good)")
    elif 25 <= bmi < 29.9:
        print("Overweight")
    else:
        print("Obese")
    print()

    fasting_sugar = float(input("Enter fasting blood sugar level (mg/dL): "))
    post_meal_sugar = float(input("Enter post-meal blood sugar level (mg/dL): "))

    systolic_bp = int(input("Enter systolic BP (upper number): "))
    diastolic_bp = int(input("Enter diastolic BP (lower number): "))

    history_diabetes = input("Do you have a history of diabetes? (yes/no): ").lower()

    symptoms = {
        'frequent_thirst': input("Do you often feel thirsty? (yes/no): "),
        'frequent_urination': input("Do you urinate frequently? (yes/no): "),
        'blurred_vision': input("Do you have blurred vision? (yes/no): "),
        'fatigue': input("Do you feel tired often? (yes/no): "),
        'chest_pain': input("Do you experience chest pain? (yes/no): "),
        'headache': input("Do you experience frequent headaches? (yes/no): ")
    }

    print("\n======= DIAGNOSIS REPORT =======\n")
    diabetes_risk = evaluate_diabetes(fasting_sugar, post_meal_sugar, history_diabetes, symptoms)
    hypertension_risk = evaluate_hypertension(systolic_bp, diastolic_bp, symptoms)
    mild_warning = general_symptom_check(diabetes_risk, hypertension_risk, symptoms)
    final_summary(age, diabetes_risk, hypertension_risk, mild_warning)

# Entry point
if __name__ == "__main__":
    main()
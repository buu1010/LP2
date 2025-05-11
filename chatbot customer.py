import streamlit as st
import nltk
from nltk.chat.util import Chat, reflections

# Download required NLTK data
nltk.download('punkt')

# Define chatbot pairs
pairs = [
    [
        r"hi|hello|hey",
        ["Hello! How can I assist you today?", "Hi there! What can I help you with?"]
    ],
    [
        r"(.*)(hours|timing|open|close)(.*)",
        ["Our store is open from 9 AM to 9 PM, Monday to Saturday."]
    ],
    [
        r"(.*)(location|address|where)(.*)",
        ["We are located at 123 Main Street, Springfield."]
    ],
    [
        r"(.*)(services|products|offerings)(.*)",
        ["We offer a range of electronics, mobile accessories, and repair services."]
    ],
    [
        r"(.*)(phones|laptops|devices|electronics)(.*)",
        ["We sell smartphones, tablets, laptops, headphones, chargers, and more."]
    ],
    [
        r"(.*)(repair|fix|broken|service)(.*)",
        ["Yes, we offer repair services for phones, tablets, and laptops. Screen replacements, battery issues, and water damage repair are available."]
    ],
    [
        r"(.*)(price|cost|charge)(.*)(repair|service)(.*)",
        ["Repair costs vary by device and issue. Basic repairs start at $30. Please visit our store or call us for a quote."]
    ],
    [
        r"(.*)(warranty|guarantee)(.*)",
        ["All products come with a 1-year warranty. Repair services come with a 3-month service warranty."]
    ],
    [
        r"(.*)(home delivery|shipping|deliver|online order)(.*)",
        ["Yes, we provide home delivery within the city. Delivery is free for orders above $100."]
    ],
    [
        r"(.*)(contact|phone|email)(.*)",
        ["You can contact us at support@example.com or call 123-456-7890."]
    ],
    [
        r"bye|goodbye|exit",
        ["Goodbye! Have a great day!", "Thanks for visiting. Bye!"]
    ],
    [
        r"(.*)",
        ["I'm sorry, I didn't understand that. Can you please rephrase?"]
    ]
]

# Create chatbot instance
chatbot = Chat(pairs, reflections)

# Streamlit UI
st.set_page_config(page_title="Customer Chatbot", layout="centered")
st.title("Customer Service Chatbot")

# Store chat history in session state
if "chat_history" not in st.session_state:
    st.session_state.chat_history = []

# Input from user
user_input = st.text_input("You :", key="user_input")

if user_input:
    response = chatbot.respond(user_input)
    st.session_state.chat_history.append(("You", user_input))
    st.session_state.chat_history.append(("Bot", response))

# Display conversation
for sender, message in st.session_state.chat_history:
    if sender == "You":
        st.markdown(f"**You:** {message}")
    else:
        st.markdown(f"**Bot:** {message}")

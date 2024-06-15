from flask import Flask, request, jsonify

app = Flask(__name__)

latest_data = {}

@app.route('/', methods=['POST'])
def receive_data():
    data = request.get_json()
    if not data:
        app.logger.error("Data tidak disediakan")
        return jsonify({"error": "Data tidak disediakan"}), 400

    temperature = data.get('temperature')
    humidity = data.get('humidity')

    if temperature is None or humidity is None:
        app.logger.error("Data tidak valid: %s", data)
        return jsonify({"error": "Data tidak valid"}), 400

    global latest_data
    latest_data = data

    app.logger.info(f"Menerima suhu: {temperature} °C, kelembaban: {humidity} %")

    return jsonify({"message": "Data diterima"}), 200

@app.route('/latest', methods=['GET'])
def get_latest_data():
    return jsonify(latest_data), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=2207, debug=True)

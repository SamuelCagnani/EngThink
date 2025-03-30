import tkinter as tk
from tkinter import messagebox
import serial
import time

# Configuração da comunicação serial (ajuste a porta conforme necessário)
PORTA_SERIAL = "/dev/ttyACM0"  # No Linux/Mac pode ser algo como '/dev/ttyUSB0', se você estiver usando Windows, verifique no gerenciador de dispositivos algo como "COM3"
BAUD_RATE = 9600

def conectar_serial():
    try:
        return serial.Serial(PORTA_SERIAL, BAUD_RATE, timeout=1)
    except Exception as e:
        messagebox.showerror("Erro", f"Falha ao conectar na porta serial: {e}")
        return None

arduino = conectar_serial()

def enviar_senha():
    senha = senha_entry.get()
    if not senha:
        messagebox.showwarning("Aviso", "Digite uma senha!")
        return
    
    if arduino:
        arduino.write((senha + "\n").encode())
        time.sleep(2)  
        resposta = arduino.readline().decode('utf-8').strip() 

        # Limpar o campo após tentativa
        senha_entry.delete(0, tk.END)
        
        if resposta == "Sistema desbloqueado":
            # Recebe o tempo decorrido do Arduino
            tempo_decorrido = arduino.readline().decode('utf-8').strip()
            messagebox.showinfo("Resposta do Sistema", f"Sistema desbloqueado!\nTempo: {tempo_decorrido}")
        elif resposta == "Sistema bloqueado":
            messagebox.showinfo("Resposta do Sistema", "Sistema bloqueado! Acesso negado.")
        else:
            messagebox.showwarning("Aviso", "Senha incorreta. Sistema bloqueado.")
    else:
        messagebox.showerror("Erro", "Conexão com Arduino não estabelecida.")

def iniciar_sistema():
    if arduino:
        arduino.write(b's')  # Enviar 's' para iniciar
        time.sleep(2)  
        resposta = arduino.readline().decode('utf-8').strip()  
        
        # Limpar o campo após inicialização
        senha_entry.delete(0, tk.END)
        
        if resposta == "Sistema bloqueado":
            messagebox.showinfo("Sistema", "Sistema bloqueado!")
        else:
            messagebox.showinfo("Sistema", resposta)

def encerrar():
    if arduino:
        arduino.close()
    root.destroy()

# Criando interface gráfica
root = tk.Tk()
root.title("Sistema de Segurança")
root.geometry("400x300")
root.configure(bg="black")

# Centralizando os componentes
frame = tk.Frame(root, bg="black")
frame.pack(expand=True)

# Widgets
tk.Label(frame, text="Digite a Senha:", fg="green", bg="black", font=("Courier", 12)).pack(pady=10)
senha_entry = tk.Entry(frame, show="*", width=20, font=("Courier", 12))
senha_entry.pack(pady=10)

btn_enviar = tk.Button(frame, text="Enviar Senha", command=enviar_senha, font=("Courier", 12), bg="green", fg="black")
btn_enviar.pack(pady=5)

btn_iniciar = tk.Button(frame, text="Iniciar Sistema", command=iniciar_sistema, font=("Courier", 12), bg="green", fg="black")
btn_iniciar.pack(pady=5)

btn_sair = tk.Button(frame, text="Sair", command=encerrar, font=("Courier", 12), bg="red", fg="black")
btn_sair.pack(pady=5)

root.mainloop()

from django.db import models
from django.contrib.auth.models import Group

# Create your models here.
class Sensor(models.Model):
    nome = models.CharField(max_length=250)
    grupo = models.ForeignKey(Group,on_delete=models.CASCADE )

    def __str__(self):
        return self.nome
    
class Amostra(models.Model):
    sensor = models.ForeignKey(Sensor,on_delete=models.CASCADE)
    timestamp = models.DateTimeField(auto_now_add=True)
    unidade = models.CharField(max_length=250)
    valor = models.FloatField()

    def __str__(self):
        return str(self.timestamp) + " : " + self.sensor.nome + " (" + str(self.valor) + " " + self.unidade + ")"

    class Meta:
        ordering = ['timestamp']

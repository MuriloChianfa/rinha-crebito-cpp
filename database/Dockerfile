# From MySQL fixed version
FROM mariadb:10.1.48

# Expose default MySQL port
EXPOSE 3306/tcp

# Setting default settings
ENV MARIADB_AUTO_UPGRADE 0
ENV MARIADB_INITDB_SKIP_TZINFO 1
ENV MARIADB_DISABLE_UPGRADE_BACKUP 1
ENV MARIADB_ALLOW_EMPTY_ROOT_PASSWORD 1
ENV MYSQL_ALLOW_EMPTY_PASSWORD 1

# Setting default options
ENV MYSQL_USER rinha
ENV MYSQL_DATABASE rinha

# Fix main image broken
RUN rm /etc/apt/sources.list.d/mariadb.list

# Install basic dependencies
RUN apt-get update && \
    apt-get install --no-install-recommends -y \
    default-libmysqlclient-dev \
    ca-certificates \
    libbsd-dev \
    pkg-config \
    unzip \
    wget \
    zip && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Binding all addresses to MySQL listen
RUN echo "bind-address = 0.0.0.0" >> /etc/mysql/my.cnf

# Addind all dev scripts to entrypoint
ADD priority*-all-*.sql /docker-entrypoint-initdb.d/

# Changing permission of these scripts 
RUN chown -R mysql:mysql /docker-entrypoint-initdb.d/

# Creating MySQL featured folder
RUN mkdir -p /var/lib/mysql-files && \
    chown -R mysql:mysql /var/lib/mysql-files && \
    chmod 700 /var/lib/mysql-files

# Adding custom conf to MySQL service
COPY my.cnf /etc/mysql/my.cnf
